#include "pch.h"
#include "Window.h"

#include <d2d1.h>

#include "Font.h"
#include "RenderContext.h"
#include "ImageLoader.h"
#include "Structs.h"
#include "FrameResources.h"

namespace gce
{

	Window::Window(WStringView const title, int32 const width, int32 const height, FullScreenMode const mode) :
		m_HInstance(GetModuleHandle(nullptr)),
		m_windowHandle(nullptr),
		m_width(width),
		m_height(height),
		m_initialFullScreenMode(mode)
	{
		Window::Create(title, width, height, mode);
	}

	Window::~Window()
	{
		m_pSwapChain->Release();
		for (ID3D12Resource* pBuffer : m_pSwapChainBuffers) pBuffer->Release();
		m_pSwapChainBuffersHeap->Release();

		if (RenderContext::IsUIModuleEnabled())
		{
			RenderContext::GetD11Device()->ReleaseWrappedResources(m_pWrappedTarget, s_swapChainBufferCount);
			Release2DTarget();

			m_pActiveBrush= nullptr;
		}

		DestroyWindow(m_windowHandle);
		UnregisterClassW(reinterpret_cast<LPCWSTR>(m_windowClass), m_HInstance);
	}

	void Window::Create(WStringView const title, int32 const width, int32 const height, FullScreenMode const mode)
	{
		m_height = height;
		m_width = width;
		m_initialFullScreenMode = mode;
		CreateWindowClass(title, width, height);
		CreateSwapChain();
		CreateSwapChainBuffersHeap();
		RetrieveSwapChainBuffers();

		if (RenderContext::IsUIModuleEnabled()) 
		{
			Retrieve2DTargets();
		}

		ShowWindow(m_windowHandle, SW_SHOW);
		UpdateWindow(m_windowHandle);

		// If the user requested fullscreen at creation, apply it now.
		if (m_initialFullScreenMode != WINDOWED)
		{
			SetFullScreen(m_initialFullScreenMode);
		}

		m_isOpen = true;
	}

	LRESULT Window::MainWndProc(const HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	LRESULT Window::SubclassProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam, UINT_PTR uIdSubclass,
		DWORD_PTR dwRefData)
	{
		return reinterpret_cast<Window*>(dwRefData)->HandleEvent(hWnd, uMsg, wParam, lParam);
	}

	LRESULT Window::HandleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		LPMINMAXINFO lpmm;
		switch (msg)
		{
			case WM_GETMINMAXINFO:
				lpmm = reinterpret_cast<LPMINMAXINFO>(lParam);
				lpmm->ptMinTrackSize.x = 300;
				lpmm->ptMinTrackSize.y = 300;
				return 0;
			case WM_SIZE:
				m_width = LOWORD(lParam);
				m_height = HIWORD(lParam);
				m_width = max(1, m_width);
				m_height = max(1, m_height);
				ResizeWindow(m_width, m_height);

				return 0;
			case WM_DESTROY:
				//FrameResourcesManager::Instance().EndFrame();
				FrameResourcesManager::Instance().Flush();
				PostQuitMessage(0);
				m_isOpen = false;
				return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void Window::ResizeViewport()
	{
		m_viewPort.TopLeftX = m_viewPort.TopLeftY = 0;
		m_viewPort.Width = static_cast<float32>(m_width);
		m_viewPort.Height = static_cast<float32>(m_height);
		m_viewPort.MinDepth = 0.f;
		m_viewPort.MaxDepth = 1.f;

		m_scissorRect.left = m_scissorRect.top = 0;
		m_scissorRect.right = m_width;
		m_scissorRect.bottom = m_height;
	}

	void Window::ResizeWindow(int32 const newWidth, int32 const newHeight)
	{
		FrameResourcesManager::Instance().Flush();
		ReleaseSwapChainBuffers();
		if (RenderContext::IsUIModuleEnabled()) Release2DTarget();

		m_pSwapChain->ResizeBuffers(s_swapChainBufferCount, newWidth, newHeight, BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		ResizeViewport();

		RetrieveSwapChainBuffers();
		if (RenderContext::IsUIModuleEnabled()) Retrieve2DTargets();
		m_currentBackBuffer = 0;

		if (m_ResizeCallBack) m_ResizeCallBack(this);
		OnWindowResize();
	}

	bool Window::CreateWindowClass(WStringView const title, int32 const width, int32 const height)
	{

		WNDCLASS wc{};
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_HInstance;
		wc.lpfnWndProc = MainWndProc;
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = title.data();

		m_windowClass = RegisterClass(&wc);
		if (!m_windowClass)
		{
			PRINT_DEBUG("RegisterClass Failed !")
				return false;
		}

		m_windowHandle = CreateWindow(title.data(), title.data(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width,
			height, 0, 0, m_HInstance, 0);
		if (!m_windowHandle)
		{
			PRINT_DEBUG("Failed to create window !")
				return false;
		}


		SetWindowSubclass(m_windowHandle, SubclassProc, 1, reinterpret_cast<DWORD_PTR>(this));

		return true;
	}

	bool Window::CreateSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = m_width;
		sd.BufferDesc.Height = m_height;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Format = BackBufferFormat;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = s_swapChainBufferCount;
		sd.OutputWindow = m_windowHandle;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT res = RenderContext::GetDXGIFactory()->CreateSwapChain(FrameResourcesManager::GetCmdQueue(), &sd, &m_pSwapChain);
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create swap chain", res);
			return false;
		}

		RenderContext::GetDXGIFactory()->MakeWindowAssociation(m_windowHandle, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
		return true;
	}

	bool Window::CreateSwapChainBuffersHeap()
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descHeapDesc.NumDescriptors = s_swapChainBufferCount;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descHeapDesc.NodeMask = 0;

		HRESULT res = RenderContext::GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_pSwapChainBuffersHeap));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create swap chain buffers heap", res);
			return false;
		}

		m_pSwapChainBuffersHeap->SetName(L"Swapchain buffers heap");

		CD3DX12_CPU_DESCRIPTOR_HANDLE firstHandle(m_pSwapChainBuffersHeap->GetCPUDescriptorHandleForHeapStart());
		for (int32 i = 0; i < s_swapChainBufferCount; i++)
		{
			m_pRTVHandles[i] = firstHandle;
			firstHandle.Offset(1, RenderContext::RtvDescriptorSize);
		}

		return true;
	}

	bool Window::RetrieveSwapChainBuffers()
	{
		for (int32 i = 0; i < s_swapChainBufferCount; i++)
		{
			m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pSwapChainBuffers[i]));

			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
			rtvDesc.Format = BackBufferFormat;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = 0;
			rtvDesc.Texture2D.PlaneSlice = 0;
			RenderContext::GetDevice()->CreateRenderTargetView(m_pSwapChainBuffers[i], &rtvDesc, m_pRTVHandles[i]);
		}

		return true;
	}

	bool Window::ReleaseSwapChainBuffers()
	{
		for (int32 i = 0; i < s_swapChainBufferCount; i++) 
		{
			m_pSwapChainBuffers[i]->Release();
			m_pSwapChainBuffers[i] = nullptr;
		}
		return true;
	}

	bool Window::Release2DTarget()
	{
		RenderContext::Get2DDeviceContext()->SetTarget(nullptr);
		for (int16 buffer = 0; buffer < s_swapChainBufferCount; buffer++)
		{
			if (m_p2dRenderTarget[buffer])
			{
				m_p2dRenderTarget[buffer]->Release();
				m_p2dRenderTarget[buffer] = nullptr;
			}

			if (m_pWrappedTarget[buffer])
			{
				m_pWrappedTarget[buffer]->Release();
				m_pWrappedTarget[buffer] = nullptr;
			}
		}
		RenderContext::GetD11DeviceContext()->Flush();
		return true;
	}

	bool Window::Retrieve2DTargets()
	{
		float32 dpi = static_cast<float32>(GetDpiForWindow(m_windowHandle));
		D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(BackBufferFormat, D2D1_ALPHA_MODE_IGNORE),
			dpi,
			dpi);

		for (uint8 i = 0; i < s_swapChainBufferCount; i++)
		{

			D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
			ThrowIfFailed(RenderContext::GetD11Device()->CreateWrappedResource(
				m_pSwapChainBuffers[i],
				&d3d11Flags,
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PRESENT,
				IID_PPV_ARGS(&m_pWrappedTarget[i])
			), "Failed to create 2D Wrapped target");

			//Create a 2D2 bitmap to draw on
			Microsoft::WRL::ComPtr<IDXGISurface> surface;
			ThrowIfFailed(m_pWrappedTarget[i]->QueryInterface(IID_PPV_ARGS(&surface)), "Failed as surface");
			ThrowIfFailed(RenderContext::Get2DDeviceContext()->CreateBitmapFromDxgiSurface(
				surface.Get(),
				&bitmapProperties,
				&m_p2dRenderTarget[i]
			), "Failed to create 2D bitmap");
		}
		return true;
	}

	void Window::Update() const
	{
		MSG msg;
		while (PeekMessageW(&msg, m_windowHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void Window::Display()
	{
		m_pSwapChain->Present(0, 0);
		m_currentBackBuffer = (m_currentBackBuffer + 1) % s_swapChainBufferCount;
	}

	bool Window::IsFullscreen() const
	{
		BOOL res;
		m_pSwapChain->GetFullscreenState(&res, nullptr);
		return res;
	}

	void Window::SetTitle(std::wstring_view const& title) const
	{
		SetWindowText(m_windowHandle, title.data());
	}

	void Window::SetFullScreen(FullScreenMode const mode)
	{
		// Compute target window rectangle for fullscreen/borderless
		RECT rect = {0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
		AdjustWindowRect(&rect, WS_POPUP | WS_VISIBLE, false);

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		
		DXGI_MODE_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.Format = BackBufferFormat;
		desc.RefreshRate.Numerator = 144;
		desc.RefreshRate.Denominator = 1;
		desc.Scaling = DXGI_MODE_SCALING_STRETCHED;
		
		// Resize target mode first (best-effort)
		m_pSwapChain->ResizeTarget(&desc);

		// Current fullscreen state
		BOOL state;
		m_pSwapChain->GetFullscreenState(&state, nullptr);

		switch (mode)
		{
		case EXCLUSIVE_FS:
			// request exclusive fullscreen
			if (FAILED(m_pSwapChain->SetFullscreenState(true, nullptr)))
			{
				PRINT_DEBUG("Failed to enter exclusive fullscreen via SetFullscreenState");
			}
			SetWindowLong(m_windowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(m_windowHandle, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED);
			break;
		case BORDERLESS:
			// borderless window (windowed but without chrome and covering the screen)
			if (state) m_pSwapChain->SetFullscreenState(false, nullptr);
			SetWindowLong(m_windowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(m_windowHandle, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED);
			break;
		case WINDOWED_MAXIMISE:
			if (state) m_pSwapChain->SetFullscreenState(false, nullptr);
			SetWindowLong(m_windowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
			ShowWindow(m_windowHandle, SW_MAXIMIZE);
			break;
		case WINDOWED:
			if (state) m_pSwapChain->SetFullscreenState(false, nullptr);
			SetWindowLong(m_windowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
			ShowWindow(m_windowHandle, SW_RESTORE);
			break;
		}

		// restore swapchain target refresh fallback
		desc.RefreshRate.Numerator = 0;
		m_pSwapChain->ResizeTarget(&desc);

		// Ensure swapchain buffers and viewport are consistent with new size
		ResizeWindow(width, height);
	}

	void Window::BeginUI()
	{
		// Acquire our wrapped render target resource for the current back buffer.

		RenderContext::GetD11DeviceContext()->ClearState();
		RenderContext::GetD11Device()->AcquireWrappedResources(&m_pWrappedTarget[m_currentBackBuffer], 1);
		RenderContext::Get2DDeviceContext()->SetTarget(m_p2dRenderTarget[m_currentBackBuffer]);
		RenderContext::Get2DDeviceContext()->BeginDraw();
	}

	void Window::SetActiveBrush(UiBrush& brush)
	{
		m_pActiveBrush = brush.GetBrush();
	}

	void Window::DrawTextUI(std::wstring text, Font const& font, RectanglePosF const& rectanglePos, Vector2f32 const& scale, float32 const RotationAngle)
	{
		D2D1_SIZE_F rtSize = m_p2dRenderTarget[m_currentBackBuffer]->GetSize();
		//D2D1_RECT_F textRect = D2D1::RectF(0, 0, rtSize.width/2.0f, rtSize.height/2.0f);

		RenderContext::Get2DDeviceContext()->SetTransform(D2D1::Matrix3x2F::Scale(scale.x, scale.y) * D2D1::Matrix3x2F::Rotation(RotationAngle) * D2D1::Matrix3x2F::Translation(rectanglePos.left, rectanglePos.top));
		RenderContext::Get2DDeviceContext()->DrawTextW(
			text.data(),
			static_cast<uint32>(text.size()),
			font.m_pFormat,
			rectanglePos,
			m_pActiveBrush);
		RenderContext::Get2DDeviceContext()->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void Window::DrawShapeUI(ShapeUi& shape, bool fill)
	{
		switch (shape.GetType())
		{
		case gce::ShapeUi::RECT:
			if (!fill) RenderContext::Get2DDeviceContext()->DrawRectangle(*dynamic_cast<RectanglePosF*>(&shape), m_pActiveBrush);
			if (fill)  RenderContext::Get2DDeviceContext()->FillRectangle(*dynamic_cast<RectanglePosF*>(&shape), m_pActiveBrush);
			break;
		case gce::ShapeUi::ROUND_RECT:
			if (!fill) RenderContext::Get2DDeviceContext()->DrawRoundedRectangle(*dynamic_cast<RoundRectanglePosF*>(&shape), m_pActiveBrush);
			if (fill)  RenderContext::Get2DDeviceContext()->FillRoundedRectangle(*dynamic_cast<RoundRectanglePosF*>(&shape), m_pActiveBrush);
			break;
		case gce::ShapeUi::ELLIPSE:
			if (!fill) RenderContext::Get2DDeviceContext()->DrawEllipse(*dynamic_cast<Ellipse*>(&shape), m_pActiveBrush);
			if (fill)  RenderContext::Get2DDeviceContext()->FillEllipse(*dynamic_cast<Ellipse*>(&shape), m_pActiveBrush);
			break;
		case gce::ShapeUi::LINE:
			RenderContext::Get2DDeviceContext()->DrawLine(dynamic_cast<Line*>(&shape)->Start(), dynamic_cast<Line*>(&shape)->End(), m_pActiveBrush, dynamic_cast<Line*>(&shape)->width);
			break;
		}
	}

	void Window::EndUI()
	{
		RenderContext::Get2DDeviceContext()->EndDraw();
		RenderContext::Get2DDeviceContext()->SetTarget(nullptr);
		RenderContext::GetD11Device()->ReleaseWrappedResources(&m_pWrappedTarget[m_currentBackBuffer], 1);
		RenderContext::GetD11DeviceContext()->Flush();
	}
}
