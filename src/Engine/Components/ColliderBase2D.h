#ifndef ENGINE_COLLIDER_BASE_2D_H_INCLUDED
#define ENGINE_COLLIDER_BASE_2D_H_INCLUDED

#include "Event.hpp"
#include "define.h"
#include "Base.h"

namespace gce {


DECLARE_COMPONENT_BASE( ColliderBase2D, ComponentBase )
{
	Event<GameObject*> collision2DEnterEvent;
	Event<GameObject*> collision2DStayEvent;
	Event<GameObject*> collision2DExitEvent;

	Vector2f32 m_localOffSet{ 0,0 };
	bool isTrigger = false;
	bool isColliding = false;

	Vector<uint32> m_wasCollidingID; //TODO change to macro

protected:
	virtual void UpdateColliderComponent(uint8 dirty) = 0;

	friend class PhysicSystem;
};


}

#endif