#ifndef RENDER_RENDER_TARGET_H_INCLUDED
#define RENDER_RENDER_TARGET_H_INCLUDED

#include <d3d12.h>
#include <DirectXMath.h>
#include "define.h"
#include "DynamicBuffer.h"
#include "Texture.h"
#include "Structs.h"

namespace gce
{
	class DynamicBuffer;
	class Window;
	class D12ComputePipelineObject;
	class D12PipelineObject;
	class Geometry;
	class RenderCamera;
	class CubeMap;

	class RenderTarget
	{
	public:

		RenderTarget(uint32 const width, uint32 const height, Window& window);
		RenderTarget(RenderTarget const& other) = delete;
		RenderTarget(RenderTarget&&) noexcept = delete;

		auto operator=(RenderTarget const& other) = delete;
		auto operator=(RenderTarget&& other) = delete;

		~RenderTarget();

		void SetCamera(RenderCamera& camera);

		virtual void Begin3D();
		virtual void Draw(RenderItem const& item);
		virtual void DrawSkyBox(CubeMap const& skybox);
		virtual void End();

		void Resize(uint32 width, uint32 height);

	protected:
		void CreateRTVHeap();
		void CreateDSVHeap();

		void RetrieveRTBuffer();
		void RetrieveDSBuffer();

		void CreateViewport();
		void UpdateViewport();

	protected:
		ID3D12Resource* m_pRenderTargetBuffer = nullptr;
		ID3D12Resource* m_pDepthStencilBuffer = nullptr;
		ID3D12DescriptorHeap* m_pRTVHeap = nullptr;
		ID3D12DescriptorHeap* m_pDSVHeap = nullptr;
		ID3D12GraphicsCommandList* m_pFrameCmdList = nullptr;

		D3D12_RECT m_scissorRect = {};
		D3D12_VIEWPORT m_viewport = {};

		uint32 m_width = 0;
		uint32 m_height = 0;

		uint32 m_lightCount = 0;
		LightPassData m_lightPassData;

		Window* m_pWindow;

		uint64 m_activeCamera = 0;

		friend class RenderWindow;
		friend class Window;
		friend class SplitScreenWindow;
	};

}

#endif