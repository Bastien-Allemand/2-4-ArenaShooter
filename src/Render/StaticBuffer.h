#ifndef RENDER_STATIC_BUFFER_H_INCLUDED
#define RENDER_STATIC_BUFFER_H_INCLUDED

#include <d3d12.h>
#include "define.h"

namespace gce
{
	class Texture;
	class ImageLoader;
	struct ImageData;

	class StaticBuffer
	{
	public:
		StaticBuffer() = default;
		StaticBuffer(StaticBuffer const& other) = delete;
		StaticBuffer(StaticBuffer&& other) noexcept = delete;

		StaticBuffer& operator=(StaticBuffer const& other) = delete;
		StaticBuffer& operator=(StaticBuffer&& other) = delete;

		~StaticBuffer();

		void Init(void const* initData, uint64 initDataSize);
		void InitTexture(const ImageData& imgData, bool is3D = false);
		void SetName(std::wstring_view name) const { m_pDefaultBuffer->SetName(name.data()); }

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress() const { return m_pDefaultBuffer->GetGPUVirtualAddress(); }
	private:
		ID3D12Resource* m_pDefaultBuffer = nullptr;
		ID3D12Resource* m_pUploadBuffer = nullptr;

		uint64 m_dataSize = 0;

		friend class RenderContext;
		friend class Texture;
		friend class Texture3D;
		friend class CubeMap;
		friend class ImageLoader;
	};
}

#endif