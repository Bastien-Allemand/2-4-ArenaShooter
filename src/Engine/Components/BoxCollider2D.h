#ifndef ENGINE_BOX_COLLIDER_2D_H_INCLUDED
#define ENGINE_BOX_COLLIDER_2D_H_INCLUDED

#include "define.h"
#include "ColliderBase2D.h"
#include "Maths/BoundingBox.h"
#include "Maths/Vector2.hpp"

namespace gce {


DECLARE_COMPONENT_FAMILY( BoxCollider2D, ColliderBase2D, CollidersMask )
{
public:
	void Init() override;
	Vector2f32 localScale{ 1.f,1.f };

	Vector<Vector2f32> localOBBPoints; //without Rot n Scale

	//bool m_dirty_offset;
	//bool m_dirty_scale; //#TODO dirty mask

protected:
	void UpdateColliderComponent(uint8 dirty) override;
private:
	Square m_worldBox;
	friend class PhysicSystem;
};


}

#endif