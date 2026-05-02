#include "pch.h"
#include "D12ComputePipelineObject.h"

#include "D12PipelineObject.h"
#include "RenderContext.h"
#include "Shader.h"

namespace gce
{

	D12ComputePipelineObject::D12ComputePipelineObject(Shader const& shader, D12PipelineObject& rootsig)
	{
		D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.pRootSignature = rootsig.m_pRootSignature;
		psoDesc.CS = { shader.GetData(), shader.GetSize() };
		psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		HRESULT res = RenderContext::GetDevice()->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&m_pPipelineState));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Error while creating compute PSO", res);
			return;
		}
	}

	D12ComputePipelineObject::D12ComputePipelineObject(D12ComputePipelineObject const& other)
	{
		m_pPipelineState = other.m_pPipelineState;
		m_pPipelineState->AddRef();
	}

	D12ComputePipelineObject& D12ComputePipelineObject::operator=(D12ComputePipelineObject const& other)
	{
		if (this != &other)
		{
			if (m_pPipelineState) m_pPipelineState->Release();

			m_pPipelineState = other.m_pPipelineState;
			m_pPipelineState->AddRef();
		}
		return *this;
	}

	D12ComputePipelineObject::~D12ComputePipelineObject()
	{
		m_pPipelineState->Release();
	}
	
}
