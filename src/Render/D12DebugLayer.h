#ifndef RENDER_DEBUG_LAYER_H_INLCUDED
#define RENDER_DEBUG_LAYER_H_INLCUDED

struct ID3D12Debug;
struct IDXGIDebug1;

namespace gce
{
	
	class D12DebugLayer
	{
	public:
		D12DebugLayer();
		D12DebugLayer(D12DebugLayer const& other) = delete;
		D12DebugLayer(D12DebugLayer&& other) noexcept = delete;

		auto operator=(D12DebugLayer const& other) = delete;
		auto operator=(D12DebugLayer&& other) = delete;

		~D12DebugLayer();

	private:
		ID3D12Debug* m_pDebugLayer = nullptr;
		IDXGIDebug1* m_pDxgiDebugLayer = nullptr;
	};

}
#endif