#ifndef RENDER_CUBE_MAP_H_INCLUDED
#define RENDER_CUBE_MAP_H_INCLUDED

#include "D12PipelineObject.h"
#include "Texture.h"
#include "Structs.h"
#include "DynamicBuffer.h"
#include "Maths/Vector3.h"

namespace gce
{
	class D12PipelineObject;
	class Geometry;
	class Camera;

	class CubeMap
	{
	public:
		CubeMap() = default;
		CubeMap(std::string const& path, Vector3f32 const& scale = Vector3f32(20.0f,20.0f,20.0f));

		CubeMap& operator=(CubeMap const& other) = delete;
		CubeMap& operator=(CubeMap&& other) = delete;

		~CubeMap();

		void LockSkyBoxOnCamera(uint32 isLockOnCamera);

		void Create(std::string const& path, Vector3f32 const& scale = Vector3f32(20.0f, 20.0f, 20.0f));

		void SetWorldMatrix(Matrix matrix);

		D12PipelineObject const& GetPso() const;
		Texture const& GetTexture() const;
		DynamicBuffer const& GetBuffer() const;
		Geometry const& GetGeo() const;

	private:

		SkyBoxData datas;
		D12PipelineObject m_pso = {};
		Texture m_texture = {};
		DynamicBuffer m_buffer = {sizeof(SkyBoxData)};
		Geometry* m_pGeo = nullptr;
	};
}

#endif
