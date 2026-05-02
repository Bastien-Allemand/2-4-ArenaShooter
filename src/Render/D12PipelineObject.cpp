#include "pch.h"
#include "D12PipelineObject.h"

#include "RenderContext.h"
#include "Shader.h"
#include "Window.h"

namespace gce
{
	D12PipelineObject::D12PipelineObject(Shader const& VS, Shader const& PS, Shader const& rootsig, PSOFlag flag)
	{
		Init(VS, PS, rootsig, flag);
	}

	D12PipelineObject::~D12PipelineObject()
	{
		m_pRootSignature->Release();
		m_pPipelineState->Release();
	}

	void D12PipelineObject::Init(Shader const& VS, Shader const& PS, Shader const& rootsig, PSOFlag flag)
	{
		HRESULT res = RenderContext::GetDevice()->CreateRootSignature(0, rootsig.GetData(), rootsig.GetSize(), IID_PPV_ARGS(&m_pRootSignature));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create root signature", res);
			return;
		}

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.pRootSignature = m_pRootSignature;
		psoDesc.InputLayout = { m_inputLayout, _countof(m_inputLayout) };
		psoDesc.VS = { VS.GetData(), VS.GetSize() };
		psoDesc.PS = { PS.GetData(), PS.GetSize() };
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

		psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
		psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		psoDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
		psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.SampleDesc.Count = RenderContext::GetMSAACount();
		psoDesc.SampleDesc.Quality = RenderContext::GetMSAAQualityLevel();
		psoDesc.RTVFormats[0] = Window::BackBufferFormat;
		psoDesc.DSVFormat = Window::DepthStencilFormat;

		if (flag & FLAG_WIREFRAME)
		{
			psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		}

		if (flag & FLAG_SKYBOX)
		{
			psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		}

		res = RenderContext::GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPipelineState));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create pipeline state object", res);
			return;
		}
	}

	D12PipelineObject::D12PipelineObject(Shader const& VS, Shader const& PS, Shader const& HS, Shader const& DS, Shader const& rootsig, PSOFlag flag)
	{
		HRESULT res = RenderContext::GetDevice()->CreateRootSignature(0, rootsig.GetData(), rootsig.GetSize(), IID_PPV_ARGS(&m_pRootSignature));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create root signature", res);
			return;
		}

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.pRootSignature = m_pRootSignature;
		psoDesc.InputLayout = { m_inputLayout, _countof(m_inputLayout) };
		psoDesc.VS = { VS.GetData(), VS.GetSize() };
		psoDesc.PS = { PS.GetData(), PS.GetSize() };
		psoDesc.HS = { HS.GetData(), HS.GetSize() };
		psoDesc.DS = { DS.GetData(), DS.GetSize() };
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

		psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
		psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
		psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		psoDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
		psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
		psoDesc.NumRenderTargets = 1;
		psoDesc.SampleDesc.Count = RenderContext::GetMSAACount();
		psoDesc.SampleDesc.Quality = RenderContext::GetMSAAQualityLevel();
		psoDesc.RTVFormats[0] = Window::BackBufferFormat;
		psoDesc.DSVFormat = Window::DepthStencilFormat;

		if (flag & FLAG_WIREFRAME)
		{
			psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		}

		if (flag & FLAG_SKYBOX)
		{
			psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		}

		res = RenderContext::GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPipelineState));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Failed to create pipeline state object", res);
			return;
		}
	}
	
}

