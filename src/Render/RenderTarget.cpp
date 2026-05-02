#include "pch.h"
#include "RenderTarget.h"

#include "FrameResources.h"
#include "RenderCamera.h"
#include "D12PipelineObject.h"
#include "Geometry.h"
#include "GeometryFactory.h"
#include "RenderContext.h"
#include "Window.h"
#include "CubeMap.h"
#include "Maths.h"
#include "Structs.h"


namespace gce
{
	RenderTarget::RenderTarget(uint32 const width, uint32 const height, Window& window) : m_width(width), m_height(height), m_pWindow(&window)
	{
		CreateRTVHeap();
		CreateDSVHeap();

		CreateViewport();

		RetrieveRTBuffer();
		RetrieveDSBuffer();
	}

	void RenderTarget::CreateDSVHeap()
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.NodeMask = 0;
		descHeapDesc.NumDescriptors = 1;
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

		HRESULT res = RenderContext::GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_pDSVHeap));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create depth stencil heap", res);
		}
	}

	void RenderTarget::RetrieveDSBuffer()
	{

		CD3DX12_HEAP_PROPERTIES dsvHeapProp(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC depthStencilDesc(D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			0,
			m_width,
			m_height,
			1,
			1,
			Window::DepthStencilFormat,
			RenderContext::GetMSAACount(),
			RenderContext::GetMSAAQualityLevel(),
			D3D12_TEXTURE_LAYOUT_UNKNOWN,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		D3D12_CLEAR_VALUE optClear = {};
		optClear.Format = Window::DepthStencilFormat;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;

		HRESULT res = RenderContext::GetDevice()->CreateCommittedResource(&dsvHeapProp, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &optClear, IID_PPV_ARGS(&m_pDepthStencilBuffer));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to retrieve DSV on render target", res);
			return;
		}

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = Window::DepthStencilFormat;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		RenderContext::GetDevice()->CreateDepthStencilView(m_pDepthStencilBuffer, &dsvDesc, m_pDSVHeap->GetCPUDescriptorHandleForHeapStart());
	}


	void RenderTarget::CreateRTVHeap()
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descHeapDesc.NumDescriptors = 1;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descHeapDesc.NodeMask = 0;

		HRESULT res = RenderContext::GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_pRTVHeap));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create render target heap", res);
		}
	}

	void RenderTarget::RetrieveRTBuffer()
	{
		CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			Window::BackBufferFormat,
			max(1,m_width),
			max(1,m_height),
			1,
			1,
			RenderContext::GetMSAACount(),
			RenderContext::GetMSAAQualityLevel(),
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);

		D3D12_CLEAR_VALUE optClear = {};
		optClear.Format = Window::BackBufferFormat;
		memcpy(optClear.Color, Window::sClearColor, sizeof(float) * 4);

		HRESULT res = RenderContext::GetDevice()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			RESOURCE_STATE_COPY_SOURCE,
			&optClear,
			IID_PPV_ARGS(&m_pRenderTargetBuffer)
		);
		m_pRenderTargetBuffer->SetName(L"Render Target Texture");

		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create render target", res);
		}

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = Window::BackBufferFormat;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;

		RenderContext::GetDevice()->CreateRenderTargetView(m_pRenderTargetBuffer, &rtvDesc, m_pRTVHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void RenderTarget::CreateViewport()
	{
		m_viewport.TopLeftX = m_viewport.TopLeftY = 0;
		m_viewport.Width = static_cast<float32>(m_width);
		m_viewport.Height = static_cast<float32>(m_height);
		m_viewport.MinDepth = 0.f;
		m_viewport.MaxDepth = 1.f;

		m_scissorRect.left = m_scissorRect.top = 0;
		m_scissorRect.right = static_cast<int32>(m_width);
		m_scissorRect.bottom = static_cast<int32>(m_height);
	}

	void RenderTarget::UpdateViewport()
	{
		ID3D12GraphicsCommandList* cmdList = FrameResourcesManager::GetCmdList();
		cmdList->RSSetViewports(1, &m_viewport);
		cmdList->RSSetScissorRects(1, &m_scissorRect);
	}

	void RenderTarget::SetCamera(RenderCamera& camera)
	{
		camera.UpdateVPMatrix();

		PassData pd = {};
		pd.viewProj = camera.m_viewProj;
		pd.invView = camera.m_world;
		
		Vector3f32 vec;
		Quaternion quat;
		camera.m_world.Decompose(pd.eyePos, vec, quat);
		
		if (camera.m_cameraId == -1)
			FrameResourcesManager::AddCamera( camera );

		m_activeCamera = camera.m_cameraId;
		FrameResourcesManager::GetCurrentFrameResource()->m_pPassCB[camera.m_cameraId]->CopyData(pd);
	}

	void RenderTarget::Resize(uint32 width, uint32 height)
	{
		m_pRenderTargetBuffer->Release();
		m_pDepthStencilBuffer->Release();

		m_width = width;
		m_height = height;

		CreateViewport();

		RetrieveRTBuffer();
		RetrieveDSBuffer();
	}

	void RenderTarget::Begin3D()
	{
		m_pFrameCmdList = FrameResourcesManager::GetCmdList();

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pRenderTargetBuffer,
			RESOURCE_STATE_COPY_SOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

		m_pFrameCmdList->ResourceBarrier(1, &barrier);

		D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_pRTVHeap->GetCPUDescriptorHandleForHeapStart();
		D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pDSVHeap->GetCPUDescriptorHandleForHeapStart();

		m_pFrameCmdList->OMSetRenderTargets(1, &rtv, false, &dsv);

		m_pFrameCmdList->RSSetViewports(1, &m_viewport);
		m_pFrameCmdList->RSSetScissorRects(1, &m_scissorRect);

		m_pFrameCmdList->ClearRenderTargetView(rtv, Window::sClearColor, 0, nullptr);
		m_pFrameCmdList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		m_pFrameCmdList->SetDescriptorHeaps(1, &RenderContext::Instance().m_pTexturesHeap);
	}

	void RenderTarget::Draw(RenderItem const& item)
	{
		m_pFrameCmdList->SetGraphicsRootSignature(item.pPso->m_pRootSignature);
		m_pFrameCmdList->SetPipelineState(item.pPso->m_pPipelineState);

		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = FrameResourcesManager::GetCurrentFrameResource()->m_pObjDatasCB[item.id]->GetGPUAddress();
		m_pFrameCmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);
		m_pFrameCmdList->SetGraphicsRootConstantBufferView(1, FrameResourcesManager::GetCurrentFrameResource()->m_pPassCB[m_activeCamera]->GetGPUAddress());
		if (FrameResourcesManager::GetCurrentFrameResource()->m_pLightsCB) m_pFrameCmdList->SetGraphicsRootConstantBufferView(2, FrameResourcesManager::GetCurrentFrameResource()->m_pLightsCB->GetGPUAddress());

		D3D12_VERTEX_BUFFER_VIEW vertexBuffer = item.pGeometry->GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW indexBuffer = item.pGeometry->GetIndexBufferView();

		m_pFrameCmdList->IASetVertexBuffers(0, 1, &vertexBuffer);
		m_pFrameCmdList->IASetIndexBuffer(&indexBuffer);
		m_pFrameCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

		m_pFrameCmdList->SetGraphicsRootDescriptorTable(3, RenderContext::GetTexturesHeap()->GetGPUDescriptorHandleForHeapStart());
		//m_frameCmdList->SetGraphicsRoot32BitConstant(4, item.pObjData->material.ambientTextureID, 0);

		m_pFrameCmdList->DrawIndexedInstanced(static_cast<UINT>(item.pGeometry->GetIndicesCount()), 1, 0, 0, 0);
	}

	void RenderTarget::DrawSkyBox(CubeMap const& skybox)
	{
		m_pFrameCmdList->SetGraphicsRootSignature(skybox.GetPso().m_pRootSignature);
		m_pFrameCmdList->SetPipelineState(skybox.GetPso().m_pPipelineState);

		m_pFrameCmdList->SetGraphicsRootConstantBufferView(0, skybox.GetBuffer().GetGPUAddress());
		m_pFrameCmdList->SetGraphicsRootConstantBufferView(1, FrameResourcesManager::GetCurrentFrameResource()->m_pPassCB[m_activeCamera]->GetGPUAddress());
		//m_frameCmdList->SetGraphicsRootConstantBufferView(2, FrameResourcesManager::GetCurrentFrameResource()->m_pLightsCB->GetGPUAddress());

		D3D12_VERTEX_BUFFER_VIEW vertexBuffer = skybox.GetGeo().GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW indexBuffer = skybox.GetGeo().GetIndexBufferView();

		m_pFrameCmdList->IASetVertexBuffers(0, 1, &vertexBuffer);
		m_pFrameCmdList->IASetIndexBuffer(&indexBuffer);

		m_pFrameCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_pFrameCmdList->SetGraphicsRootDescriptorTable(2, RenderContext::GetTexturesHeap()->GetGPUDescriptorHandleForHeapStart());

		m_pFrameCmdList->SetGraphicsRoot32BitConstant(3, skybox.GetTexture().GetTextureID(), 0);

		m_pFrameCmdList->DrawIndexedInstanced(static_cast<UINT>(skybox.GetGeo().GetIndicesCount()), 1, 0, 0, 0);
	}

	void RenderTarget::End()
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pRenderTargetBuffer,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			RESOURCE_STATE_COPY_SOURCE);
		m_pFrameCmdList->ResourceBarrier(1, &barrier);
	}

	RenderTarget::~RenderTarget()
	{
		m_pDepthStencilBuffer->Release();
		m_pRenderTargetBuffer->Release();
		m_pRTVHeap->Release();
		m_pDSVHeap->Release();
	}

	
}