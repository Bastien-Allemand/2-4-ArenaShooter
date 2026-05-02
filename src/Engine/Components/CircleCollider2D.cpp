#include "pch.h"
#include "CircleCollider2D.h"
#include "GameObject.h"
#include "GameTransform.h"
#include "MeshRenderer.h"
#include "Geometry.h"

namespace gce {
#define CIRCLE_DEFAULT_RADIUS .5f
#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

	void CircleCollider2D::Init()
	{
		m_pOwner->pCollision2DEvents = &collision2DEnterEvent;
		m_pOwner->transform.dirtyEvent.AddListener(&CircleCollider2D::UpdateColliderComponent, this);

		if (!GetOwner().HasComponent<gce::MeshRenderer>())
		{
			localRadius = CIRCLE_DEFAULT_RADIUS;
			return;
		}
		Geometry const* geo = GetOwner().GetComponent<gce::MeshRenderer>()->GetGeometry();

		localRadius = (geo->max.x - geo->min.x) * .5f;
		float value = (geo->max.y - geo->min.y) * .5f;
		if (value > localRadius) localRadius = value;
	}

	void CircleCollider2D::UpdateColliderComponent(uint8 const dirty)
	{
		GameTransform& transform = m_pOwner->transform;

		m_worldCircle.center = {
			transform.GetWorldPosition().x + m_localOffSet.x * transform.GetWorldScale().x,
			transform.GetWorldPosition().y + m_localOffSet.y * transform.GetWorldScale().y 
		};

		const Vector2f32& scale = { transform.GetWorldScale().x, transform.GetWorldScale().y };
		float circleRadiusScale = MAX(scale.x, scale.y);

		m_worldCircle.radius = localRadius * localScale * circleRadiusScale;
	}

#undef CIRCLE_DEFAULT_RADIUS
} // namespace gce