//------------------------------------------------------------------------------
//
// Kompactor.hpp created by Yyhrs 2020/10/24
//
//------------------------------------------------------------------------------

#ifndef KOMPACTOR_HPP
#define KOMPACTOR_HPP

#include <QFileInfo>

#include "assets/Models.h"
#include "general/GameTypes.h"
#include "general/Objects.h"

class Kompactor
{
public:
	struct Report
	{
		QString name;
		quint64 oldVerticesNumber;
		quint64 newVerticesNumber;
		quint64 oldFaceNumber;
		quint64 newFaceNumber;
	};

	Kompactor(const QFileInfo &input, const QFileInfo &output, int vertexPrecision, int normalPrecision, const QStringList &shaders);
	~Kompactor() = default;

	QList<Report> excute();

private:
	void rebuildShadow(const Alamo::Model::SubMesh &subMesh, Buffer<Alamo::MASTER_VERTEX> &vertices, Buffer<uint16_t> &indices);
	void writeShadow(QFile &fileIn, QFile &fileOut, Alamo::Model &model, const Alamo::Model::ShaderEntryPoint &entryPoint, Buffer<Alamo::MASTER_VERTEX> &vertices, Buffer<uint16_t> &indices);

	using Key = std::tuple<QString, QString, QString, QString, QString, QString>;
	using Face = std::tuple<uint16_t, uint16_t, uint16_t>;

	QFileInfo   m_input;
	QFileInfo   m_output;
	int         m_vertexPrecision{3};
	int         m_normalPrecision{3};
	QStringList m_shaders;
};

#endif // KOMPACTOR_HPP
