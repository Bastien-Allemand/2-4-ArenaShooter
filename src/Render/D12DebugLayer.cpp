#include "pch.h"
#include "D12DebugLayer.h"
#include "define.h"

namespace gce
{
	
	D12DebugLayer::D12DebugLayer()
	{
		HRESULT res = D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebugLayer));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Error while creating d3d12 debug interface", res);
			return;
		}

		res = DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_pDxgiDebugLayer));
		if (FAILED(res))
		{
			PRINT_COM_ERROR("Error while creating dxgi debug interface", res);
			return;
		}

		m_pDebugLayer->EnableDebugLayer();
	}

	D12DebugLayer::~D12DebugLayer()
	{
		PRINT_DEBUG("DXGI Reports living device objects:");
		if (m_pDxgiDebugLayer) m_pDxgiDebugLayer->ReportLiveObjects(DXGI_DEBUG_ALL, static_cast<DXGI_DEBUG_RLO_FLAGS>(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));

		m_pDebugLayer->Release();
		m_pDxgiDebugLayer->Release();
	}
}
