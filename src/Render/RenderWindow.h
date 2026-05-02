#ifndef RENDER_RENDER_WINDOW_H_INCLUDED
#define RENDER_RENDER_WINDOW_H_INCLUDED

#include "DynamicBuffer.h"
#include "RenderTarget.h"
#include "Window.h"
#include "Structs.h"

namespace gce
{

	class D12PipelineObject;
	class Geometry;
	class RenderCamera;
	class DynamicBuffer;

	class RenderWindow : public Window // to see public Window + public RenderTarget
	{
	public:
		RenderWindow() = default;
		RenderWindow(WStringView title, int32 width, int32 height, FullScreenMode mode = WINDOWED);
		RenderWindow(RenderWindow const& other) = delete;
		RenderWindow(RenderWindow&& other) noexcept = delete;

		auto operator=(RenderWindow const& other) = delete;
		auto operator=(RenderWindow&& other) = delete;

		~RenderWindow() override;

		void Create(WStringView title, int32 width, int32 height, FullScreenMode mode = WINDOWED) override;
		void SetCamera(RenderCamera& camera) const override { m_pTarget->SetCamera(camera); }


		void Begin3D() override;
		void Draw(RenderItem const& item) override;
		void EndDraw() override;

		RenderTarget* GetRenderTarget() override;

	private:
		RenderTarget* m_pTarget = nullptr;
		ID3D12GraphicsCommandList* m_pFrameCmdList = nullptr;

	private:
		void OnWindowResize() override;
	};
	
}

#endif