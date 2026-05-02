#include "pch.h"
#include "Maths.h"
#include "RenderCamera.h"

namespace gce
{
	void RenderCamera::UpdateVPMatrix()
	{

		switch (m_type)
		{
		case PERSPECTIVE:
			m_viewProj = Matrix().PerspectiveFovLH(
				perspective.fov,
				perspective.aspectRatio,
				perspective.nearPlane,
				perspective.farPlane);


			break;

		case TWO_DIMENSIONAL:
		case ORTHOGRAPHIC:
			m_viewProj = m_proj.OrthographicLH(
				orthographic.viewWidth, orthographic.viewHeight,
				orthographic.nearPlane, orthographic.farPlane);
			break;

		default:
			break;
		}


		Matrix view = m_world.GetInverse();
		m_viewProj = view * m_viewProj;
		m_viewProj.Transpose();
	}
}