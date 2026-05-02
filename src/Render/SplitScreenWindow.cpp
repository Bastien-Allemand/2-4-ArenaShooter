#include "pch.h"
#include "SplitScreenWindow.h"
#include "FrameResources.h"

#include "D12PipelineObject.h"
#include "DynamicBuffer.h"

#include "RenderContext.h"
#include "RenderTarget.h"
#include "Shader.h"
#include "Structs.h"

namespace gce
{
	SplitScreenWindow::SplitScreenWindow(WStringView title, uint32 width, uint32 height, SplitScreenDisposition splitScreenForm, FullScreenMode mode)
	{
		Create(title, width, height, splitScreenForm, mode);
	}


	SplitScreenWindow::~SplitScreenWindow()
	{
		delete m_pRenderTarget;
		m_scissorsList.Clear();
		m_viewportList.Clear();
	}

	/////////////////////////////////////////////////////////////////////
	/// @brief Window Creation.
	/// @param title Window's title
	/// @param width Window's width
	/// @param height Window's height 
	/// @param splitScreenForm Choosen SplitScreen disposition preset
	/////////////////////////////////////////////////////////////////////

	void SplitScreenWindow::Create(WStringView title, uint32 width, uint32 height, SplitScreenDisposition splitScreenForm, FullScreenMode mode)
	{
		m_form = splitScreenForm;

		m_pRenderTarget = new RenderTarget(width, height, *this);
		switch (splitScreenForm)
		{
			case HORIZONTAL_2_PLAYERS:
			case VERTICAL_2_PLAYERS:
				m_screenCount = 2;
				break;

			case HORIZONTAL_4_PLAYERS:
			case VERTICAL_4_PLAYERS:
			case SQUARE_4_PLAYERS:
				m_screenCount = 4;
				break;
			case SQUARE_6_PLAYERS:
				m_screenCount = 6;
				break;
			case SQUARE_8_PLAYERS:
				m_screenCount = 8;
				break;
		}

		for (int8 i = 0; i < m_screenCount; i++)
		{
			m_viewportList.std.emplace_back();
			m_scissorsList.std.emplace_back();
		}

		ResizeViewports();
		Window::Create(title, width, height, mode);
	}


	////////////////////////////////////////////////
	/// @brief Begin Drawing for all the screens
	////////////////////////////////////////////////
	void SplitScreenWindow::Begin3D()
	{
		m_pFrameCmdList = FrameResourcesManager::GetCmdList();

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			RESOURCE_STATE_COPY_DEST);
		m_pFrameCmdList->ResourceBarrier(1, &barrier);

		m_pRenderTarget->Begin3D();
	}


	////////////////////////////////////////////////
	/// @brief Choose the screen to draw on
	/// @param screenID Id of the choosen screen
	////////////////////////////////////////////////
	void SplitScreenWindow::SetScreenToDraw(uint8 screenID)
	{
		// Screen choosen out of range
		if (screenID >= m_viewportList.Size()) return;

		m_pRenderTarget->m_viewport = m_viewportList[screenID];
		m_pRenderTarget->m_scissorRect = m_scissorsList[screenID];
		m_pRenderTarget->UpdateViewport();
	}

	void SplitScreenWindow::Draw(RenderItem const& item)
	{
		m_pRenderTarget->Draw(item);
	}


	////////////////////////////////////////////////
	/// @brief End Drawing for all the screens
	////////////////////////////////////////////////
	void SplitScreenWindow::EndDraw()
	{
		m_pRenderTarget->End();

		switch (RenderContext::GetMSAACount())
		{
		case NONE:
			//use CopyResource() to copy resource without MSAA
			m_pFrameCmdList->CopyResource(m_pSwapChainBuffers[m_currentBackBuffer], m_pRenderTarget->m_pRenderTargetBuffer);
			break;

		default:
			m_pFrameCmdList->ResolveSubresource(m_pSwapChainBuffers[m_currentBackBuffer], 0, m_pRenderTarget->m_pRenderTargetBuffer, 0, BackBufferFormat);
			break;
		}

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pSwapChainBuffers[m_currentBackBuffer],
			RESOURCE_STATE_COPY_DEST,
			RenderContext::IsUIModuleEnabled() ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_PRESENT);
		FrameResourcesManager::GetCmdList()->ResourceBarrier(1, &barrier);
	}


	//////////////////////////////////////////////////////
	/// @brief Resize the Rendertarget and the screens
	//////////////////////////////////////////////////////
	void SplitScreenWindow::OnWindowResize()
	{
		m_pRenderTarget->Resize(m_width , m_height);
		ResizeViewports();
	}


	/////////////////////////////////
	/// @brief Resize the screens
	/////////////////////////////////
	void SplitScreenWindow::ResizeViewports()
	{
		switch (m_form)
		{
		case HORIZONTAL_2_PLAYERS:
			for (uint8 i = 0; i < m_screenCount; i++)
			{
				D3D12_VIEWPORT& viewPort = m_viewportList[i];
				viewPort.TopLeftX = 0.0f;
				viewPort.TopLeftY = static_cast<float32>(i) * (static_cast<float32>(m_height) * 0.5f);
				viewPort.Width    = static_cast<float32>(m_width);
				viewPort.Height   = static_cast<float32>(m_height) * 0.5f;
				viewPort.MinDepth = 0.f;
				viewPort.MaxDepth = 1.f;

				D3D12_RECT& scissorRect = m_scissorsList[i];
				scissorRect.left   = 0l;
				scissorRect.top    = static_cast<int32>(static_cast<float32>(i) * (static_cast<float32>(m_height) * 0.5f));
				scissorRect.right  = static_cast<int32>(m_width);
				scissorRect.bottom = static_cast<int32>(static_cast<float32>(m_height) * 0.5f + (static_cast<float32>(i) * static_cast<float32>(m_height) * 0.5f));
			}
			break;
		case VERTICAL_2_PLAYERS:
			for (uint8 i = 0; i < m_screenCount; i++)
			{
				D3D12_VIEWPORT& viewPort = m_viewportList[i];

				viewPort.TopLeftX = static_cast<float32>(i) * (static_cast<float32>(m_width) * 0.5f);
				viewPort.TopLeftY = 0.0f;
				viewPort.Width    = static_cast<float32>(m_width) * 0.5f;
				viewPort.Height   = static_cast<float32>(m_height);
				viewPort.MinDepth = 0.f;
				viewPort.MaxDepth = 1.f;
				m_viewportList.PushBack(viewPort);

				D3D12_RECT& scissorRect = m_scissorsList[i];

				scissorRect.left   = static_cast<int32>(static_cast<float32>(i) * (static_cast<float32>(m_width) * 0.5f));
				scissorRect.top    = 0l;
				scissorRect.right  = static_cast<int32>(static_cast<float32>(m_width) * 0.5f + (static_cast<float32>(i) * static_cast<float32>(m_width) * 0.5f));
				scissorRect.bottom = static_cast<int32>(m_height);

			}
			break;
		case VERTICAL_4_PLAYERS:
			for (uint8 i = 0; i < m_screenCount; i++)
			{
				D3D12_VIEWPORT& viewPort = m_viewportList[i];

				viewPort.TopLeftX = static_cast<float32>(i) * (static_cast<float32>(m_width) * 0.25f);
				viewPort.TopLeftY = 0.0f;
				viewPort.Width    = static_cast<float32>(m_width) * 0.25f;
				viewPort.Height   = static_cast<float32>(m_height);
				viewPort.MinDepth = 0.f;
				viewPort.MaxDepth = 1.f;

				D3D12_RECT& scissorRect = m_scissorsList[i];

				scissorRect.left   = static_cast<int32>(static_cast<float32>(i) * (static_cast<float32>(m_width) * 0.25f));
				scissorRect.top    = 0l;
				scissorRect.right  = static_cast<int32>(static_cast<float32>(m_width) * 0.25f + (static_cast<float32>(i) * m_width * 0.25f));
				scissorRect.bottom = static_cast<int32>(m_height);
			}
			break;
		case HORIZONTAL_4_PLAYERS:
			for (uint8 i = 0; i < m_screenCount; i++)
			{
				D3D12_VIEWPORT& viewPort = m_viewportList[i];

				viewPort.TopLeftX = static_cast<float32>(i) * (static_cast<float32>(m_width) * 0.25f);
				viewPort.TopLeftY = 0;
				viewPort.Width    = static_cast<float32>(m_width) * 0.25f;
				viewPort.Height   = static_cast<float32>(m_height);
				viewPort.MinDepth = 0.0f;
				viewPort.MaxDepth = 1.f;

				D3D12_RECT& scissorRect = m_scissorsList[i];

				scissorRect.left   = static_cast<int32>(static_cast<float32>(i) * (static_cast<float32>(m_width) * 0.25f));
				scissorRect.top    = 0l;
				scissorRect.right  = static_cast<int32>(static_cast<float32>(m_width) * 0.25f + (static_cast<float32>(i) * m_width * 0.25f));
				scissorRect.bottom = static_cast<int32>(m_height);
			}
			break;
		case SQUARE_4_PLAYERS:
			for (uint8 i = 0; i < m_screenCount; i++)
			{
				D3D12_VIEWPORT& viewPort = m_viewportList[i];
				viewPort.TopLeftX = static_cast<float32>(i % 2) * (static_cast<float32>(m_width) * 0.5f);
				viewPort.TopLeftY = static_cast<float32>(i > 1) * (static_cast<float32>(m_height) * 0.5f);
				viewPort.Width    = static_cast<float32>(m_width) * 0.5f;
				viewPort.Height   = static_cast<float32>(m_height) * 0.5f;
				viewPort.MinDepth = 0.0f;
				viewPort.MaxDepth = 1.f;

				D3D12_RECT& scissorRect = m_scissorsList[i];
				scissorRect.left   = static_cast<int32>(static_cast<float32>(i % 2) * (static_cast<float32>(m_width) * 0.5f));
				scissorRect.top    = static_cast<int32>(static_cast<float32>(i > 1) * (static_cast<float32>(m_height) * 0.5f));
				scissorRect.right  = static_cast<int32>(static_cast<float32>(m_width) * 0.5f + (static_cast<float32>(i % 2) * (static_cast<float32>(m_width) * 0.5f)));
				scissorRect.bottom = static_cast<int32>(static_cast<float32>(m_height) + (static_cast<float32>(i > 1) * (static_cast<float32>(m_height) * 0.5f)));
			}
			break;
		case SQUARE_6_PLAYERS:

			for (uint8 i = 0; i < m_screenCount; i++)
			{
				D3D12_VIEWPORT& viewPort = m_viewportList[i];
				viewPort.TopLeftX = static_cast<float32>(i % 3) * (static_cast<float32>(m_width) / 3.0f);
				viewPort.TopLeftY = static_cast<float32>(i > 2) * (static_cast<float32>(m_height) * 0.5f);
				viewPort.Width    = static_cast<float32>(m_width) / 3.0f;
				viewPort.Height   = static_cast<float32>(m_height) * 0.5f;
				viewPort.MinDepth = 0.f;
				viewPort.MaxDepth = 1.f;

				D3D12_RECT& scissorRect = m_scissorsList[i];
				scissorRect.left   = static_cast<int32>(static_cast<float32>(i % 3) * (static_cast<float32>(m_width) / 3.0f));
				scissorRect.top    = static_cast<int32>(static_cast<float32>(i > 2) * (static_cast<float32>(m_height) * 0.5f));
				scissorRect.right  = static_cast<int32>(static_cast<float32>(m_width) / 3.0f + (static_cast<float32>(i % 3) * (static_cast<float32>(m_width) /3.0f)));
				scissorRect.bottom = static_cast<int32>(static_cast<float32>(m_height) + (static_cast<float32>(i > 2) * (static_cast<float32>(m_height) * 0.5f)));
			}
			break;
		case SQUARE_8_PLAYERS:
			for (uint8 i = 0; i < m_screenCount; i++)
			{
				D3D12_VIEWPORT& viewPort = m_viewportList[i];
				viewPort.TopLeftX = static_cast<float32>(i % 4) * (static_cast<float32>(m_width) * 0.25f);
				viewPort.TopLeftY = static_cast<float32>(i > 3) * (static_cast<float32>(m_height) * 0.5f);
				viewPort.Width    = static_cast<float32>(static_cast<float32>(m_width) * 0.25f);
				viewPort.Height   = static_cast<float32>(static_cast<float32>(m_height) * 0.5f);
				viewPort.MinDepth = 0.f;
				viewPort.MaxDepth = 1.f;

				D3D12_RECT& scissorRect = m_scissorsList[i];
				scissorRect.left   = static_cast<int32>(static_cast<float32>(i % 4) * (static_cast<float32>(m_width) * 0.25f));
				scissorRect.top    = static_cast<int32>(static_cast<float32>(i > 3) * (static_cast<float32>(m_height) * 0.5f));
				scissorRect.right  = static_cast<int32>(static_cast<float32>(m_width) * 0.25f + (static_cast<float32>(i % 4) * (static_cast<float32>(m_width) * 0.25f)));
				scissorRect.bottom = static_cast<int32>(static_cast<float32>(m_height) + (static_cast<float32>(i > 3) * (static_cast<float32>(m_height) * 0.5f)));
			}
			break;
		}
	}
	

	/////////////////////////////////////////////////////////////////
	/// @brief Calculate Aspect Ratio for all screen dispositions
	/////////////////////////////////////////////////////////////////
	float32 SplitScreenWindow::GetAspectRatio() const
	{
		switch (m_form)
		{
		case HORIZONTAL_2_PLAYERS:
			return static_cast<float32>(m_width) / (static_cast<float32>(m_height) * 0.5f);
		case VERTICAL_2_PLAYERS:
			return (static_cast<float32>(m_width) * 0.5f) / static_cast<float32>(m_height);
		case HORIZONTAL_4_PLAYERS:
			return static_cast<float32>(m_width) / (static_cast<float32>(m_height) * 0.25f);
		case VERTICAL_4_PLAYERS:
			return static_cast<float32>(m_width) * 0.25f / static_cast<float32>(m_height);
		case SQUARE_4_PLAYERS:
			return static_cast<float32>(m_width) * 0.5f / (static_cast<float32>(m_height) * 0.5f);
		case SQUARE_6_PLAYERS:
			return static_cast<float32>(m_width) / 3.0f / (static_cast<float32>(m_height) * 0.5f);
		case SQUARE_8_PLAYERS:
			return static_cast<float32>(m_width) * 0.25f / (static_cast<float32>(m_height) * 0.5f);
		default:
			return 0.0f;
		}
	}


	// Getter:


	RenderTarget* SplitScreenWindow::GetRenderTarget()
	{
		return m_pRenderTarget;
	}
}


