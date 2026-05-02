#include "pch.h"
#include "CubeMap.h"

#include "ImageLoader.h"
#include "GeometryFactory.h"
#include "Texture.h"

#include "Shader.h"
#include "D12PipelineObject.h"
#include "Maths/Quaternion.h"
#include "Maths/Matrix.h"

namespace gce
{	
	/////////////////////////////////////////////////////////////////////
	/// @brief Skybox Creation.
	/// @param path Path of the Skybox's texture in res file
	/// @param scale Scale of the Skybox set at 20 if not set by user
	/////////////////////////////////////////////////////////////////////
	CubeMap::CubeMap(std::string const& path, Vector3f32 const& scale)
	{
		Create(path);
	}

	CubeMap::~CubeMap()
	{
		delete m_pGeo;
	}

	void CubeMap::LockSkyBoxOnCamera(uint32 isLockOnCamera)
	{
		datas.isLockOncamera = isLockOnCamera;
		m_buffer.CopyData(datas);
	}

	//////////////////////////////////////////////////////////////////////
	/// @brief Skybox Creation.
	/// @param path Path of the Skybox's texture in res file
	/// @param scale Scale of the Skybox set at 20 if not set by user
	/// @note Skybox store it's own pso and texture
	//////////////////////////////////////////////////////////////////////
	void CubeMap::Create(std::string const& path, Vector3f32 const& scale)
	{
		Shader vs("res/Render/Shaders/Skybox.vs.cso");
		Shader ps("res/Render/Shaders/Skybox.ps.cso");
		Shader rootSig("res/Render/Shaders/Skybox.rs.cso");

		m_pso.Init(vs, ps, rootSig, FLAG_SKYBOX);

		m_texture.Create3D(path);
		Matrix worldMatrix = Matrix::Scale(scale) * Matrix::Rotation({}) * Matrix::Translation({0.0f,0.0f,0.0f});
		if (m_pGeo != nullptr) delete m_pGeo;

		m_pGeo = GeometryFactory::CreateCubeGeo();

		datas.objData.world = worldMatrix.GetTransposed();
		datas.isLockOncamera = 1;

		m_buffer.CopyData(datas);
	}

	void CubeMap::SetWorldMatrix(Matrix matrix)
	{
		datas.objData.world = matrix.GetTransposed();
		m_buffer.CopyData(datas);
	}

	// Getter :

	D12PipelineObject const& CubeMap::GetPso() const
	{
		return m_pso;
	}
	Texture const& CubeMap::GetTexture() const
	{
		return m_texture;
	}
	DynamicBuffer const& CubeMap::GetBuffer() const
	{
		return m_buffer;
	}
	Geometry const& CubeMap::GetGeo() const
	{
		return *m_pGeo;
	}
}
