#ifndef RENDER_READ_BACK_BUFFER_HPP_INCLUDED
#define RENDER_READ_BACK_BUFFER_HPP_INCLUDED

#include <d3d12.h>
#include "d3dx12.h"
#include "RenderContext.h"
#include "define.h"

namespace gce
{
	
	template <typename T>
	class ReadbackBuffer
	{
	public:
		ReadbackBuffer() = default;
		ReadbackBuffer(uint32 size);

		ReadbackBuffer(ReadbackBuffer const& other) = delete;
		ReadbackBuffer(ReadbackBuffer&& other) noexcept = delete;

		ReadbackBuffer& operator=(ReadbackBuffer const& other) = delete;
		ReadbackBuffer& operator=(ReadbackBuffer&& other) = delete;

		~ReadbackBuffer();

		void Create(uint32 size);
		T* GetData() { return m_pData; }
		ID3D12Resource* GetResource() const { return m_buffer; }

	private:
		ID3D12Resource* m_pBuffer;
		T* m_pData;
	};

	template <typename T>
	ReadbackBuffer<T>::ReadbackBuffer(uint32 const size)
	{
		Create(size);
	}

	template <typename T>
	void ReadbackBuffer<T>::Create(uint32 const size)
	{
		CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);
		CD3DX12_RESOURCE_DESC resource = CD3DX12_RESOURCE_DESC::Buffer(size);

		RenderContext::GetDevice()->CreateCommittedResource(&prop,
			D3D12_HEAP_FLAG_NONE,
			&resource,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_buffer));

		m_pBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_pData));
	}

	template <typename T>
	ReadbackBuffer<T>::~ReadbackBuffer()
	{
		m_pBuffer->Unmap(0, nullptr);
		m_pBuffer->Release();
	}

}
#endif