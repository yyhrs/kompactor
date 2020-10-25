//------------------------------------------------------------------------------
//
// Kompactor.cpp created by Yyhrs 2020/10/24
//
//------------------------------------------------------------------------------

#include <QDebug>
#include <QMap>
#include <QSet>

#include "assets/Files.h"
#include "Kompactor.hpp"

Kompactor::Kompactor(const QFileInfo &input, const QFileInfo &output, int vertexPrecision, int normalPrecision):
	m_input{input},
	m_output{output},
	m_vertexPrecision{vertexPrecision},
	m_normalPrecision{normalPrecision}
{
}

QList<Kompactor::Report> Kompactor::excute()
{
	QList<Report>     reports;
	ptr<Alamo::IFile> in{new Alamo::PhysicalFile(m_input.absoluteFilePath().toStdWString())};
	Alamo::Model      model{in};
	QFile             fileIn{m_input.absoluteFilePath()};
	QFile             fileOut{m_output.absoluteFilePath()};

	if (!fileIn.open(QFile::ReadOnly) || !fileOut.open(QFile::WriteOnly | QFile::Truncate))
		return {};
	for (const auto &entryPoint: model.shadowEntryPoints())
	{
		const auto                   &subMesh{model.GetMesh(entryPoint.mesh).subMeshes[entryPoint.submesh]};
		Buffer<Alamo::MASTER_VERTEX> vertices;
		Buffer<uint16_t>             indices;

		rebuildShadow(subMesh, vertices, indices);
		writeShadow(fileIn, fileOut, model, entryPoint, vertices, indices);
		reports << Report{QString::fromStdString(subMesh.mesh->name), subMesh.vertices.size(), vertices.size(), subMesh.indices.size() / 3, indices.size() / 3};
	}
	fileOut.write(fileIn.readAll());
	fileIn.close();
	fileOut.close();
	return reports;
}

void Kompactor::rebuildShadow(const Alamo::Model::SubMesh &subMesh, Buffer<Alamo::MASTER_VERTEX> &vertices, Buffer<uint16_t> &indices)
{
	QMap<Key, uint16_t>      keyIndices;
	QMap<uint16_t, uint16_t> newIndices;
	uint16_t                 index{0};
	QSet<Face>               faces;

	vertices.reserve(subMesh.vertices.size());
	for (size_t i = 0; i < subMesh.vertices.size(); ++i)
	{
		const Key key{QString::number(subMesh.vertices[i].Position.x, 'f', m_vertexPrecision),
					  QString::number(subMesh.vertices[i].Position.y, 'f', m_vertexPrecision),
					  QString::number(subMesh.vertices[i].Position.z, 'f', m_vertexPrecision),
					  QString::number(subMesh.vertices[i].Normal.x, 'f', m_normalPrecision),
					  QString::number(subMesh.vertices[i].Normal.y, 'f', m_normalPrecision),
					  QString::number(subMesh.vertices[i].Normal.z, 'f', m_normalPrecision)};

		if (keyIndices.contains(key))
			newIndices[i] = keyIndices[key];
		else
		{
			newIndices[i] = index;
			keyIndices[key] = index;
			vertices.append(&subMesh.vertices[i], 1);
			++index;
		}
	}
	indices.reserve(subMesh.indices.size());
	for (size_t i = 0; i < subMesh.indices.size(); i = i + 3)
	{
		Face face{newIndices.value(subMesh.indices[i]), newIndices.value(subMesh.indices[i + 1]), newIndices.value(subMesh.indices[i + 2])};

		if (std::get<0>(face) != std::get<1>(face) && std::get<0>(face) != std::get<2>(face) && std::get<1>(face) != std::get<2>(face))
		{
			indices.append(&std::get<0>(face), 1);
			indices.append(&std::get<1>(face), 1);
			indices.append(&std::get<2>(face), 1);
		}
	}
}

void Kompactor::writeShadow(QFile &fileIn, QFile &fileOut, Alamo::Model &model, const Alamo::Model::ShadowEntryPoint &entryPoint, Buffer<Alamo::MASTER_VERTEX> &vertices, Buffer<uint16_t> &indices)
{
	const auto &subMesh{model.GetMesh(entryPoint.mesh).subMeshes[entryPoint.submesh]};
	auto       difference{(subMesh.vertices.size() - vertices.size()) * sizeof(Alamo::MASTER_VERTEX) + (subMesh.indices.size() - indices.size()) * sizeof(uint16_t)};
	uint32_t   value;

	fileOut.write(fileIn.read(model.meshesEntryPoints().at(entryPoint.mesh) - 4 - fileIn.pos()));
	fileIn.read(reinterpret_cast<char *>(&value), sizeof(uint32_t));
	value = value - difference;
	fileOut.write(reinterpret_cast<char *>(&value), sizeof(uint32_t));
	fileOut.write(fileIn.read(entryPoint.size - 4 - fileIn.pos()));
	fileIn.read(reinterpret_cast<char *>(&value), sizeof(uint32_t));
	value = value - difference;
	fileOut.write(reinterpret_cast<char *>(&value), sizeof(uint32_t));
	fileOut.write(fileIn.read(entryPoint.sizesIn - fileIn.pos()));
	value = static_cast<uint32_t>(vertices.size());
	fileIn.skip(sizeof(uint32_t));
	fileOut.write(reinterpret_cast<char *>(&value), sizeof(uint32_t));
	value = static_cast<uint32_t>(indices.size() / 3);
	fileIn.skip(sizeof(uint32_t));
	fileOut.write(reinterpret_cast<char *>(&value), sizeof(uint32_t));
	fileOut.write(fileIn.read(entryPoint.vertexIn - 4 - fileIn.pos()));
	value = vertices.size() * sizeof(Alamo::MASTER_VERTEX);
	fileOut.write(reinterpret_cast<char *>(&value), sizeof(uint32_t));
	fileIn.skip(sizeof(uint32_t));
	fileOut.write(reinterpret_cast<char *>(vertices.m_data), sizeof(Alamo::MASTER_VERTEX) * vertices.size());
	fileIn.seek(entryPoint.vertexOut);
	fileOut.write(fileIn.read(entryPoint.indicesIn - 4 - fileIn.pos()));
	value = indices.size() * sizeof(uint16_t);
	fileOut.write(reinterpret_cast<char *>(&value), sizeof(uint32_t));
	fileIn.skip(sizeof(uint32_t));
	fileOut.write(reinterpret_cast<char *>(indices.m_data), sizeof(uint16_t) * indices.size());
	fileIn.seek(entryPoint.indicesOut);
}
