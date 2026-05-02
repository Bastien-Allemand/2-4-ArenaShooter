#ifndef RENDER_WINDOW_H_INCLUDED
#define RENDER_WINDOW_H_INCLUDED

#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <Windows.h>
#include <vector>
#include <functional>

#include <d3d11on12.h>
#include <d2d1_1.h>

#include "Structs.h"

namespace gce
{
	struct FrameResource;
	struct RectanglePosF;
	class Font;
	class RenderCamera;
	class Geometry;
	class D12PipelineObject;
	class DynamicBuffer;
	class Texture;
	class RenderTarget;

	enum FullScreenMode
	{
		EXCLUSIVE_FS, BORDERLESS, WINDOWED, WINDOWED_MAXIMISE
	};
	
	class Window
	{
	public:
		Window() = default;
		Window(WStringView title, int32 width, int32 height, FullScreenMode mode = WINDOWED);
		Window(Window const& other) = delete;
		Window(Window&& other) noexcept = delete;

		auto operator=(Window const& other) = delete;
		auto operator=(Window&& other) = delete;

		virtual ~Window();

		virtual void Create(WStringView title, int32 width, int32 height, FullScreenMode mode = WINDOWED);
		void Update() const;
		void Display();

		virtual float32 GetAspectRatio() const { return min(10, static_cast<float32>(m_width) / static_cast<float32>(m_height)); }
		bool IsOpen() const { return m_isOpen; }
		uint32 GetWidth() const { return m_width; }
		uint32 GetHeight() const { return m_height; }
		bool IsFullscreen() const;
		// float32 GetAspectRatio() const { return static_cast<float32>(m_width) / static_cast<float32>(m_height); }

		void OnWindowResizeCallBack(std::function<void(Window*)> const& func) { m_ResizeCallBack = func; }
		void SetTitle(std::wstring_view const& title) const;
		void SetFullScreen(FullScreenMode mode);

		//----
		virtual void Begin3D() = 0;
		virtual void SetScreenToDraw(uint8 screenID) {};
		virtual void SetCamera(RenderCamera& camera) const = 0;
		virtual void Draw(RenderItem const& item) = 0;
		virtual void EndDraw() = 0;

		virtual RenderTarget* GetRenderTarget() = 0;

		//== UI MODULE==

		void BeginUI();
		void SetActiveBrush(UiBrush& brush);
	
		void DrawShapeUI(ShapeUi& rectanglePos, bool fillRect);
		void DrawTextUI(std::wstring text, Font const&, RectanglePosF const& rectanglePos, Vector2f32 const& scale = { 1.0f,1.0f }, float32 RotationAngle = 0.0f);
		
		void EndUI();

		static constexpr DXGI_FORMAT BackBufferFormat{ DXGI_FORMAT_B8G8R8A8_UNORM };
		static constexpr DXGI_FORMAT DepthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };
		static constexpr int32 s_swapChainBufferCount { 2 };

		inline static const DirectX::XMVECTORF32 sClearColor{ DirectX::Colors::LightBlue };


	protected:
		std::vector<ID3D12CommandList*> m_pPendingLists = {};

		int32 m_currentBackBuffer = 0;
		IDXGISwapChain* m_pSwapChain = nullptr;

		D3D12_CPU_DESCRIPTOR_HANDLE m_pRTVHandles[s_swapChainBufferCount] = {};
		ID3D12Resource* m_pSwapChainBuffers[s_swapChainBufferCount] = {};
		ID3D12DescriptorHeap* m_pSwapChainBuffersHeap = nullptr;

		ID3D12Fence* m_pFence = nullptr;
		uint64 m_fenceValue = 0;

		D3D12_VIEWPORT m_viewPort = {};
		D3D12_RECT m_scissorRect = {};

		ATOM m_windowClass = {};
		HINSTANCE m_HInstance = {};
		HWND m_windowHandle = {};

		bool m_isOpen = false;
		uint32 m_width = 0;
		uint32 m_height = 0;

		std::function<void(Window*)> m_ResizeCallBack;

		// == UI ==
		ID3D11Resource* m_pWrappedTarget[s_swapChainBufferCount];
		ID2D1Bitmap1* m_p2dRenderTarget[s_swapChainBufferCount];

		//TODO Move those frame resources
		ID2D1Brush* m_pActiveBrush = nullptr;

		// Remember initial fullscreen mode requested at creation
		FullScreenMode m_initialFullScreenMode = WINDOWED;

		//============

	protected:
		ID3D12Resource* GetCurrentBackBuffer() const { return m_pSwapChainBuffers[m_currentBackBuffer]; }
		virtual void OnWindowResize() {};

		void ExecuteLists();
		void FlushCmdQueue();
		void AddPendingList(ID3D12GraphicsCommandList* list) { m_pPendingLists.push_back(list); }

	private:
		static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

		LRESULT HandleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void ResizeViewport();
		void ResizeWindow(int32 newWidth, int32 newHeight);

		bool CreateWindowClass(WStringView const title, int32 const width, int32 const height);
		bool CreateCmdObjects();
		bool CreateSwapChain();

		bool CreateSwapChainBuffersHeap();

		bool RetrieveSwapChainBuffers();
		bool ReleaseSwapChainBuffers();

		bool Init2DModules();
		bool Release2DTarget();
		bool Retrieve2DTargets();

		friend class RenderTarget;
		friend struct FrameResource;
	};
	
}
#endif