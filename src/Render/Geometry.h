#ifndef RENDER_GEOMETRY_H_INCLUDED
#define RENDER_GEOMETRY_H_INCLUDED

#include <d3d12.h>

#include "StaticBuffer.h"
#include "define.h"
#include "Maths/Vector3.h"


namespace gce
{
	struct Vertex;

	class Geometry
	{
	public:
		Geometry() = default;
		Geometry(Vertex const* vertex, uint64 vertexCount, uint32 const* indices, uint64 indicesCount);
		Geometry(Geometry const& other) = delete;
		Geometry(Geometry&& other) = delete;

		Geometry& operator=(Geometry const& other) = delete;
		Geometry& operator=(Geometry&& other) = delete;

		~Geometry() = default;

		void Initialize(Vertex const* vertex, uint64 vertexCount, uint32 const* indices, uint64 indicesCount);
		void Release() const { delete this; }
		
		Vector3f32 min;
		Vector3f32 max;

		D3D12_VERTEX_BUFFER_VIEW const& GetVertexBufferView() const { return m_vertexBufferView; }
		D3D12_INDEX_BUFFER_VIEW  const& GetIndexBufferView()  const { return m_indexBufferView; }
		uint64 GetIndicesCount() const { return m_indicesCount; }

		static constexpr DXGI_FORMAT sIndexFormat{ DXGI_FORMAT_R32_UINT };

	private:
		StaticBuffer m_vertexBuffer;
		StaticBuffer m_indexBuffer;

		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

		uint64 m_indicesCount;
	};
	
}

#endif