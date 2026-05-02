#include "pch.h"
#include "DynamicBuffer.h"

#include "RenderContext.h"

namespace gce
{

	DynamicBuffer::DynamicBuffer(uint32 size)
	{
		Init(size);
	}

	DynamicBuffer::~DynamicBuffer()
	{
		m_pBuffer->Unmap(0, nullptr);
		m_pBuffer->Release();
	}

	void DynamicBuffer::Init(uint32 size)
	{
		CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(CalcConstantBufferByteSize(size));

		RenderContext::GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&m_pBuffer));

		m_pBuffer->Map(0, nullptr, &m_pCPUPointer);
	}
}
