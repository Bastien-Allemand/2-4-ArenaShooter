#include "pch.h"
#include "RenderContext.h"

#include "ImageLoader.h"
#include "StaticBuffer.h"
#include "Window.h"

#include "DDSTextureLoader.h"

#include "FrameResources.h"

namespace gce
{

	bool RenderContext::Init()
	{
		RenderContext& context = Instance();
		HRESULT res; 

	// #ifdef _DEBUG
	// 	res = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&context.m_pDxgiFactory));
	// #else
		res = CreateDXGIFactory(IID_PPV_ARGS(&context.m_pDxgiFactory));
	//#endif

		if (FAILED(res))
		{
			PRINT_COM_ERROR("Error while creating factory", res);
			return false;
		}

		res = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&context.m_pDevice));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Error while creating device", res);
			return false;
		}

		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		res = context.m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&context.m_pCommandQueue));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create command queue", res);
			return false;
		}

		res = context.m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&context.m_pCommandAllocator));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create RenderContext command allocator", res);
			return false;
		}

		res = context.m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, context.m_pCommandAllocator, nullptr, IID_PPV_ARGS(&context.m_pCommandList));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create RenderContext command list", res);
			return false;
		}

		res = context.m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&context.m_pFence));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create fence", res);
			return false;
		}

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = 128;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		res = context.m_pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&context.m_pTexturesHeap));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create textures heap", res);
			return false;
		}
		context.m_pTexturesHeap->SetName(L"Texture Heap");
		context.m_textureHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(context.m_pTexturesHeap->GetCPUDescriptorHandleForHeapStart());

		context.m_pCommandList->Close();

		context.RtvDescriptorSize = context.m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		context.DsvDescriptorSize = context.m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		context.CbvSrvUavDescriptorSize = context.m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		return true;
	}

	bool RenderContext::InitUI()
	{
		RenderContext& context = Instance();

		D2D1_FACTORY_OPTIONS options;
#ifdef _DEBUG
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
		options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

		ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &options, reinterpret_cast<void**>(&context.m_p2dFactory)), "Failed to create 2d factory");
		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &context.m_pWriteFactory), "Failed to create 2d write factory");

		context.m_isUIModuleEnabled = true;

		//== D11On12Device Initialisation
		Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
		ID3D12CommandQueue* cmdQueue = FrameResourcesManager::GetCmdQueue();

		HRESULT res = D3D11On12CreateDevice(
			RenderContext::GetDevice(),
			/*D3D11_CREATE_DEVICE_DEBUG |*/ D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			nullptr,
			0,
			reinterpret_cast<IUnknown**>(&cmdQueue),
			1,
			0,
			d3d11Device.GetAddressOf(),
			&context.m_pD11Context,
			nullptr
		);

		ThrowIfFailed(res, "Error while creating d11 device (for the UI)");

		d3d11Device->QueryInterface(IID_PPV_ARGS(&context.m_pD11Device));

		D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
		ThrowIfFailed(context.m_pD11Device->QueryInterface(IID_PPV_ARGS(&dxgiDevice)), "Failed to create 2d factory");
		ThrowIfFailed(RenderContext::Get2D2Factory()->CreateDevice(dxgiDevice.Get(), &context.m_p2dDevice), "Failed to create 2d device");
		ThrowIfFailed(context.m_p2dDevice->CreateDeviceContext(deviceOptions, &context.m_p2dDeviceContext), "Failed to create 2d device context");

		return true;
	}

	void RenderContext::FlushCommandQueue()
	{
		RenderContext& context = Instance();

		context.m_currentFenceValue++;
		context.m_pCommandQueue->Signal(context.m_pFence, context.m_currentFenceValue);

		if (context.m_pFence->GetCompletedValue() < context.m_currentFenceValue)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, L"", false, EVENT_ALL_ACCESS);
			context.m_pFence->SetEventOnCompletion(context.m_currentFenceValue, eventHandle);
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}

	void RenderContext::UploadBuffer(StaticBuffer& buffer)
	{
		RenderContext& context = Instance();

		context.m_pCommandAllocator->Reset();
		context.m_pCommandList->Reset(context.m_pCommandAllocator, nullptr);

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(buffer.m_pDefaultBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		context.m_pCommandList->ResourceBarrier(1, &barrier);

		context.m_pCommandList->CopyBufferRegion(buffer.m_pDefaultBuffer, 0, buffer.m_pUploadBuffer, 0, buffer.m_dataSize);

		barrier = CD3DX12_RESOURCE_BARRIER::Transition(buffer.m_pDefaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		context.m_pCommandList->ResourceBarrier(1, &barrier);

		context.m_pCommandList->Close();
		ID3D12CommandList* lists[] = { context.m_pCommandList };
		context.m_pCommandQueue->ExecuteCommandLists(1, lists);

		FlushCommandQueue();
		buffer.m_pUploadBuffer->Release();
		buffer.m_pUploadBuffer = nullptr;
	}

	void RenderContext::UploadTextureBuffer(StaticBuffer& buffer, D3D12_PLACED_SUBRESOURCE_FOOTPRINT const& footprint)
	{
		RenderContext& context = Instance();

		context.m_pCommandAllocator->Reset();
		context.m_pCommandList->Reset(context.m_pCommandAllocator, nullptr);

		//CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(buffer.m_pDefaultBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		//context.m_pCommandList->ResourceBarrier(1, &barrier);

		gce::ImageLoader::CopyTextureRegion(Instance().m_pCommandList, &buffer, footprint);

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(buffer.m_pDefaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		context.m_pCommandList->ResourceBarrier(1, &barrier);

		context.m_pCommandList->Close();
		ID3D12CommandList* lists[] = { context.m_pCommandList };
		context.m_pCommandQueue->ExecuteCommandLists(1, lists);

		FlushCommandQueue();
		buffer.m_pUploadBuffer->Release();
		buffer.m_pUploadBuffer = nullptr;
	}

	void RenderContext::LoadDDS(const char16* szFileName, ID3D12Resource** pTexture, ID3D12Resource** pTextureUploadHeap, int32& m_width, int32& m_height, size_t maxsize, DDSAlphaMode* pAlphaMode)
	{
		RenderContext& context = Instance();

		context.m_pCommandAllocator->Reset();
		context.m_pCommandList->Reset(context.m_pCommandAllocator, nullptr);
		
		HRESULT res = DirectX::CreateDDSTextureFromFile12(context.m_pDevice, context.m_pCommandList, szFileName, pTexture, pTextureUploadHeap, m_width, m_height, maxsize, pAlphaMode);
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to load dds file", res);
		}

		context.m_pCommandList->Close();
		ID3D12CommandList* lists[] = { context.m_pCommandList };
		context.m_pCommandQueue->ExecuteCommandLists(1, lists);

		FlushCommandQueue();
	}

	uint32 RenderContext::GetMSAAQualityLevel()
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS desc = {};
		desc.Format = Window::BackBufferFormat;
		desc.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		desc.NumQualityLevels = 0;
		desc.SampleCount = Instance().m_MSAACount;
		Instance().m_pDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &desc, sizeof(desc));

		return desc.NumQualityLevels - 1;
	}

	RenderContext::~RenderContext()
	{
		m_pDxgiFactory->Release();
		m_pDevice->Release();
		m_pFence->Release();
		m_pCommandQueue->Release();
		m_pCommandAllocator->Release();
		m_pCommandList->Release();
		m_pTexturesHeap->Release();

		if (m_isUIModuleEnabled == true)
		{
			m_p2dFactory->Release();

			m_pD11Device->Release();
			m_pD11Context->Release();
			m_p2dDevice->Release();
			m_p2dDeviceContext->Release();
		}

	}
	
}