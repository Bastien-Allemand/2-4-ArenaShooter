#ifndef RENDER_RENDER_CONTEXT_H_INCLUDED
#define RENDER_RENDER_CONTEXT_H_INCLUDED

#ifdef _DEBUG
#include "D12DebugLayer.h"
#endif
#include "define.h"
#include "d3dx12.h"

#include <d3d11on12.h>
#include <d2d1_1.h>
#include <wrl.h>
#include <dwrite.h>


struct IDXGIFactory;
struct ID3D12Device;
struct ID3D12Fence;
struct ID3D12CommandQueue;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;


namespace gce
{
	struct ImageData;
	class StaticBuffer;

	enum MSAA_COUNT : int8
	{
		NONE = 1,
		MSAA_COUNT_X2 = 2,
		MSAA_COUNT_X4 = 4,
		MSAA_COUNT_X8 = 8
	};

	class RenderContext
	{
	public:
		RenderContext(RenderContext const& other) = delete;
		RenderContext(RenderContext&& other) = delete;

		RenderContext& operator=(RenderContext const& other) = delete;
		RenderContext& operator=(RenderContext&& other) = delete;

		static bool Init();
		static bool InitUI();
		static void FlushCommandQueue();

		static void UploadBuffer(StaticBuffer& buffer);
		static void UploadTextureBuffer(StaticBuffer& buffer, D3D12_PLACED_SUBRESOURCE_FOOTPRINT const& footprint);

		static void LoadDDS(
			const char16* szFileName,
			ID3D12Resource** pTexture, 
			ID3D12Resource** pTextureUploadHeap, 
			int32& m_width, 
			int32& m_height, size_t maxsize = 0, DDSAlphaMode* pAlphaMode = nullptr);

		static void SetMSAA(MSAA_COUNT const count) { Instance().m_MSAACount = count; }

		static IDXGIFactory* GetDXGIFactory() { return Instance().m_pDxgiFactory; }
		static ID3D12Device* GetDevice() { return Instance().m_pDevice; }
		static ID3D12Fence* GetFence() { return Instance().m_pFence; }
		static ID3D12CommandQueue* GetCommandQueue() { return Instance().m_pCommandQueue; }
		static ID3D12GraphicsCommandList* GetCommandList() { return Instance().m_pCommandList; }
		static uint64& GetCurrentFenceValue() { return Instance().m_currentFenceValue; }
		static Microsoft::WRL::ComPtr<IDWriteFactory> GetWriteFactory() { return Instance().m_pWriteFactory; }
		static ID2D1Factory1* Get2D2Factory() { return Instance().m_p2dFactory; }
		static bool IsUIModuleEnabled() { return Instance().m_isUIModuleEnabled; }

		static ID3D12DescriptorHeap* GetTexturesHeap() { return Instance().m_pTexturesHeap; }
		static CD3DX12_CPU_DESCRIPTOR_HANDLE& GetTextureHeapHandle() { return Instance().m_textureHeapHandle; }

		inline static uint32 RtvDescriptorSize;
		inline static uint32 DsvDescriptorSize;
		inline static uint32 CbvSrvUavDescriptorSize;
		static MSAA_COUNT GetMSAACount() { return Instance().m_MSAACount; }

		//== UI ==
		static ID3D11On12Device* GetD11Device() { return Instance().m_pD11Device; }
		static ID3D11DeviceContext* GetD11DeviceContext() { return Instance().m_pD11Context; }
		static ID2D1Device* Get2DDevice() { return Instance().m_p2dDevice; }
		static ID2D1DeviceContext* Get2DDeviceContext() { return Instance().m_p2dDeviceContext; }


	private:
		RenderContext() = default;

		~RenderContext();

		static RenderContext& Instance()
		{
			static RenderContext rc;
			return rc;
		}

		static uint32 GetMSAAQualityLevel();

	private:

		IDXGIFactory* m_pDxgiFactory = nullptr;
		ID3D12Device* m_pDevice = nullptr;

		//== UI ==
		Microsoft::WRL::ComPtr<IDWriteFactory> m_pWriteFactory = nullptr;
		ID2D1Factory1* m_p2dFactory = nullptr;

		ID3D11On12Device* m_pD11Device = nullptr;
		ID3D11DeviceContext* m_pD11Context = nullptr;
		ID2D1Device* m_p2dDevice = nullptr;
		ID2D1DeviceContext* m_p2dDeviceContext = nullptr;
		bool m_isUIModuleEnabled = false;


		ID3D12Fence* m_pFence = nullptr;
		uint64 m_currentFenceValue = 0;

		ID3D12CommandQueue* m_pCommandQueue = nullptr;
		ID3D12CommandAllocator* m_pCommandAllocator = nullptr;
		ID3D12GraphicsCommandList* m_pCommandList = nullptr;

		ID3D12DescriptorHeap* m_pTexturesHeap = nullptr;
		CD3DX12_CPU_DESCRIPTOR_HANDLE m_textureHeapHandle;

		MSAA_COUNT m_MSAACount = NONE;

	#ifdef _DEBUG
		D12DebugLayer m_debugLayer;
	#endif

		friend class RenderTarget;
		friend class D12PipelineObject;
		friend class Texture;
		friend class Window;
	};
	
}

#endif