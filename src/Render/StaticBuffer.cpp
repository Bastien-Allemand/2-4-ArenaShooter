#include "pch.h"
#include "StaticBuffer.h"

#include "ImageLoader.h"
#include "RenderContext.h"

namespace gce
{

	void StaticBuffer::Init(void const* initData, uint64 initDataSize)
	{
	    CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(initDataSize);

	    HRESULT res = RenderContext::GetDevice()->CreateCommittedResource(
	        &heapProps,
	        D3D12_HEAP_FLAG_NONE,
	        &resourceDesc,
	        D3D12_RESOURCE_STATE_COMMON,
	        nullptr,
	        IID_PPV_ARGS(&m_pDefaultBuffer));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create", res);
		}

	    heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(CalcConstantBufferByteSize(static_cast<UINT>(initDataSize)));
		res = RenderContext::GetDevice()->CreateCommittedResource(
	        &heapProps,
	        D3D12_HEAP_FLAG_NONE,
	        &resourceDesc,
	        D3D12_RESOURCE_STATE_GENERIC_READ,
	        nullptr,
	        IID_PPV_ARGS(&m_pUploadBuffer));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create", res);
		}

		void* pMappedPtr;
		m_pUploadBuffer->Map(0, nullptr, &pMappedPtr);
		memcpy(pMappedPtr, initData, initDataSize);
		m_pUploadBuffer->Unmap(0, nullptr);

		m_dataSize = initDataSize;

		RenderContext::UploadBuffer(*this);
	}

	void StaticBuffer::InitTexture(ImageData const& imgData, bool is3D)
	{
	    CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resourceDesc = {};
		if (is3D)
		{
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(imgData.giPixelFormat, imgData.width, imgData.height);
			resourceDesc.DepthOrArraySize = 8;
		}
		else
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(imgData.giPixelFormat, imgData.width, imgData.height);

	    RenderContext::GetDevice()->CreateCommittedResource(
	        &heapProps,
	        D3D12_HEAP_FLAG_NONE,
	        &resourceDesc,
	        D3D12_RESOURCE_STATE_COPY_DEST,
	        nullptr,
	        IID_PPV_ARGS(&m_pDefaultBuffer));

		D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
		uint32 numRows;
		uint64 rowSizeInBytes;
		uint64 totalSize;
		RenderContext::GetDevice()->GetCopyableFootprints(&resourceDesc, 0, 1, 0, &footprint, &numRows, &rowSizeInBytes, &totalSize);

	    heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	    resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(totalSize);
	    RenderContext::GetDevice()->CreateCommittedResource(
	        &heapProps,
	        D3D12_HEAP_FLAG_NONE,
	        &resourceDesc,
	        D3D12_RESOURCE_STATE_GENERIC_READ,
	        nullptr,
	        IID_PPV_ARGS(&m_pUploadBuffer));

	    uint8* mappedPtr;
	    m_pUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(& mappedPtr));

		for (uint32 row = 0; row < numRows; row++)
		{
			uint8* dst = mappedPtr + row * footprint.Footprint.RowPitch;
			const int8* src = imgData.data.Data() + row * imgData.width * imgData.bitsPerPixel;
			memcpy(dst, src, imgData.width * imgData.bitsPerPixel);
		}
	    m_pUploadBuffer->Unmap(0, nullptr);

	    RenderContext::UploadTextureBuffer(*this, footprint);
	}

	StaticBuffer::~StaticBuffer()
	{
		m_pDefaultBuffer->Release();
	}
	
}
