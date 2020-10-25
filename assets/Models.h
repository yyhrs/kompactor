#ifndef MODELS_H
#define MODELS_H

#include "assets/ChunkFile.h"
#include "general/ExactTypes.h"
#include "general/GameTypes.h"
#include <map>
#include <QList>

namespace Alamo
{

	class Model: public IObject
	{
public:
		struct ShadowEntryPoint
		{
			int    mesh{0};
			int    submesh{0};
			size_t size{0};
			size_t sizesIn{0};
			size_t sizesOut{0};
			size_t vertexIn{0};
			size_t vertexOut{0};
			size_t indicesIn{0};
			size_t indicesOut{0};
		};

		struct Bone
		{
			size_t        index;
			std::string   name;
			Bone          *parent;
			bool          visible;
			BillboardType billboard;
			Matrix        absTransform;
			Matrix        invAbsTransform;
			Matrix        relTransform;
		};

		struct Attachable
		{
			Bone*bone;
		};

		struct Mesh;

		struct SubMesh
		{
			const Mesh                   *mesh;
			std::string                  shader;
			std::vector<ShaderParameter> parameters;
			std::string                  vertexFormat;
			Buffer<MASTER_VERTEX>        vertices;
			Buffer<uint16_t>             indices;
			unsigned int                 nSkinBones;
			unsigned long                skin[MAX_NUM_SKIN_BONES];
		};

		struct Mesh: public Attachable
		{
			size_t               firstSubMesh;
			std::string          name;
			BoundingBox          bounds;
			bool                 isVisible;
			bool                 isCollidable;
			std::vector<SubMesh> subMeshes;
			size_t               nVertices;
		};

		struct Light: public Attachable
		{
			std::string name;
			LightType   type;
			Color       color;
			float       intensity;
			float       farAttenuationEnd;
			float       farAttenuationStart;
			float       hotspotSize;
			float       falloffSize;
		};

		struct Proxy: public Attachable
		{
			std::string name;
			bool        isVisible;
			bool        altDecreaseStayHidden;
			Mesh        *mesh; // The mesh to which the proxy is attached, if any
		};

		struct Dazzle: public Attachable
		{
			Vector3      position;
			Color        color;
			std::string  name;
			std::string  texture;
			float        radius;
			float        phase;
			float        bias;
			float        frequency;
			bool         nightOnly;
			bool         isVisible;
			bool         colorize;
			unsigned int texX, texY, texSize;
		};

private:
		void          ReadBone(ChunkReader&reader, Bone&bone);
		void          ReadSkeleton(ChunkReader&reader);
		void          ReadSubMesh(ChunkReader&reader, SubMesh&mesh, size_t number);
		Mesh          *ReadMesh(ChunkReader&reader);
		Light         *ReadLight(ChunkReader&reader);
		unsigned long ReadDazzle(ChunkReader&reader, Dazzle&dazzle);
		void          ReadConnections(ChunkReader&reader, const std::vector<Attachable*>&objects);
		void          Cleanup();

		std::wstring            m_name;
		std::vector<Bone>       m_bones;
		std::vector<Mesh*>      m_meshes;
		std::vector<Light*>     m_lights;
		std::vector<Proxy>      m_proxies;
		std::vector<Dazzle>     m_dazzles;
		size_t                  m_numSubMeshes;
		QList<ShadowEntryPoint> m_shadowEntryPoints;
		QList<int>              m_meshesEntryPoints;

public:
		const std::wstring&GetName() const {
			return m_name;
		}
		size_t        GetNumBones() const {
			return m_bones.size();
		}
		const Bone&GetBone(size_t i) const {
			return m_bones[i];
		}
		size_t GetBone(std::string name) const;
		size_t        GetNumDazzles() const {
			return m_dazzles.size();
		}
		const Dazzle&GetDazzle(size_t i) const {
			return m_dazzles[i];
		}
		size_t        GetNumMeshes() const {
			return m_meshes.size();
		}
		const Mesh&GetMesh(size_t i) const {
			return *m_meshes[i];
		}
		size_t        GetNumLights() const {
			return m_lights.size();
		}
		const Light&GetLight(size_t i) const {
			return *m_lights[i];
		}
		size_t        GetNumProxies() const {
			return m_proxies.size();
		}
		const Proxy&GetProxy(size_t i) const {
			return m_proxies[i];
		}

		size_t GetNumSubMeshes() const {
			return m_numSubMeshes;
		}
		inline QList<int> meshesEntryPoints() const
		{
			return m_meshesEntryPoints;
		}
		inline QList<ShadowEntryPoint> shadowEntryPoints() const
		{
			return m_shadowEntryPoints;
		}

		Model(ptr<IFile> file);
		~Model();

	};

}

#endif
