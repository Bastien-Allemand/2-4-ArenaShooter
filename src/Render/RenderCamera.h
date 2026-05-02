#ifndef RENDER_RENDER_CAMERA_H_INCLUDED
#define RENDER_RENDER_CAMERA_H_INCLUDED

#include <DirectXMath.h>
#include "DynamicBuffer.h"
#include "Structs.h"

using namespace DirectX;


namespace gce
{
	enum CAMERA_TYPE : char8
	{
		PERSPECTIVE, ORTHOGRAPHIC, TWO_DIMENSIONAL
	};

	class RenderCamera
	{
	public:

		RenderCamera() = default;
		RenderCamera(CAMERA_TYPE type) : m_type(type) {}
		// RenderCamera(RenderCamera const& other) = default;
		// RenderCamera(RenderCamera&& other) noexcept = default;
		// RenderCamera& operator=(RenderCamera const& other) = default;
		// RenderCamera& operator=(RenderCamera&& other) = default;

		~RenderCamera() = default;

		struct PerspectiveSettings
		{
			float32 fov;
			float32 aspectRatio;
			float32 nearPlane;
			float32 farPlane;
			Vector3f32 up;
		};

		struct OrthographicSettings
		{
			float32 viewWidth;
			float32 viewHeight;
			float32 nearPlane;
			float32 farPlane;
		};


		void UpdateVPMatrix();

		union {
			PerspectiveSettings perspective;
			OrthographicSettings orthographic;
			OrthographicSettings twoDimensional;
		};

		void SetMatrix(Matrix const& matrix) { m_world = matrix; UpdateVPMatrix(); }
		
		Matrix& GetMatrix() { return m_world; }
		Matrix& GetVPMatrix() { return m_viewProj; };

		Matrix& GetProjMatrix() { return m_proj; }
		CAMERA_TYPE& GetCameraType() { return m_type; }
		Matrix& GetViewMatrix() { return m_view; }

		void SetType(CAMERA_TYPE type) { m_type = type; }
		CAMERA_TYPE GetType() const { return m_type; }


	protected:

		Matrix m_world;
		Vector3f32 m_forward{ 0.0f, 0.0f, 1.0f };
		Vector3f32 m_up = { 0.0f, 1.0f, 0.0f };

		CAMERA_TYPE m_type;
		Matrix m_proj;
		Matrix m_view;
		Matrix m_viewProj;

		uint64 m_cameraId = -1;

	
	private:

		friend class RenderWindow;
		friend class RenderTarget;
		friend class FrameResourcesManager;
	};
}
#endif