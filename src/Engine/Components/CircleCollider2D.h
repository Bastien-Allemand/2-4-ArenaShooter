#ifndef ENGINE_CIRCLE_COLLIDER_2D_H_INCLUDED
#define ENGINE_CIRCLE_COLLIDER_2D_H_INCLUDED

#include "define.h"
#include "ColliderBase2D.h"
#include "Maths/Sphere.h"

namespace gce {

DECLARE_COMPONENT_FAMILY( CircleCollider2D, ColliderBase2D, CollidersMask )
{
	void Init() override;

	float32 localScale = 1.f;
	float32 localRadius = 0.5f;

protected:
	void UpdateColliderComponent(uint8 dirty) override;
private:
	Circle m_worldCircle;

	friend class PhysicSystem;
};


}

#endif