#include "pch.h"
#include "Geometry.h"

#include "Structs.h"

namespace gce
{
	
	Geometry::Geometry(Vertex const* vertex, uint64 vertexCount, uint32 const* indices, uint64 indicesCount) : m_indicesCount(indicesCount)
	{
		Initialize(vertex, vertexCount, indices, indicesCount);
	}

	void Geometry::Initialize(Vertex const* vertex, uint64 vertexCount, uint32 const* indices, uint64 indicesCount)
	{
		const uint64 vbByteSize = vertexCount * sizeof(*vertex);
		const uint64 ibByteSize = indicesCount * sizeof(*indices);

		m_vertexBuffer.Init(vertex, vbByteSize);
		m_vertexBuffer.SetName(L"Vertex Buffer");

		m_indexBuffer.Init(indices, ibByteSize);
		m_indexBuffer.SetName(L"Index Buffer");

		m_vertexBufferView.BufferLocation = m_vertexBuffer.GetGPUAddress();
		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
		m_vertexBufferView.SizeInBytes = static_cast<UINT>(vbByteSize);

		m_indexBufferView.BufferLocation = m_indexBuffer.GetGPUAddress();
		m_indexBufferView.Format = sIndexFormat;
		m_indexBufferView.SizeInBytes = static_cast<UINT>(ibByteSize);
		
		min = vertex[0].pos;
	    max = vertex[0].pos;
	
	    for (uint32 i = 1; i < vertexCount; ++i)
	    {
	        if (min.x > vertex[i].pos.x) min.x = vertex[i].pos.x;
	        if (min.y > vertex[i].pos.y) min.y = vertex[i].pos.y;
	        if (min.z > vertex[i].pos.z) min.z = vertex[i].pos.z;
	
	        if (max.x < vertex[i].pos.x) max.x = vertex[i].pos.x;
	        if (max.y < vertex[i].pos.y) max.y = vertex[i].pos.y;
	        if (max.z < vertex[i].pos.z) max.z = vertex[i].pos.z;
    	}
	}



}