#include "pch.h"
#include "RenderWindow.h"

#include "RenderContext.h"
#include "RenderTarget.h"
#include "RenderCamera.h"
#include "FrameResources.h"

namespace gce
{

	RenderWindow::RenderWindow(WStringView const title, int32 const width, int32 const height, FullScreenMode mode)
	{
		RenderWindow::Create(title, width, height, mode);
	}

	void RenderWindow::Create(WStringView const title, int32 const width, int32 const height, FullScreenMode mode)
	{
		m_pTarget = new RenderTarget(width, height, *this);
		Window::Create(title, width, height, mode);
	}

	RenderWindow::~RenderWindow()
	{
		delete m_pTarget;
	}

	void RenderWindow::Begin3D()
	{
		m_pFrameCmdList = FrameResourcesManager::GetCmdList();

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			RESOURCE_STATE_COPY_DEST);
		m_pFrameCmdList->ResourceBarrier(1, &barrier);

		m_pTarget->Begin3D();
	}

	void RenderWindow::Draw(RenderItem const& item)
	{
		m_pTarget->Draw(item);
	}


	void RenderWindow::EndDraw()
	{
		m_pTarget->End();

		switch (RenderContext::GetMSAACount())
		{
		case NONE:
			//use CopyResource() to copy resource without MSAA
			m_pFrameCmdList->CopyResource(m_pSwapChainBuffers[m_currentBackBuffer], m_pTarget->m_pRenderTargetBuffer);
			break;

		default:
			m_pFrameCmdList->ResolveSubresource(m_pSwapChainBuffers[m_currentBackBuffer], 0, m_pTarget->m_pRenderTargetBuffer, 0, BackBufferFormat);
			break;
		}

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pSwapChainBuffers[m_currentBackBuffer],
			RESOURCE_STATE_COPY_DEST,
			RenderContext::IsUIModuleEnabled() ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_PRESENT);
		FrameResourcesManager::GetCmdList()->ResourceBarrier(1, &barrier);
	}

	RenderTarget* RenderWindow::GetRenderTarget()
	{
		return m_pTarget;
	}

	void RenderWindow::OnWindowResize()
	{
		m_pTarget->Resize(m_width, m_height);
	}
}


