#ifndef ENGINE_BOX_COLLIDER_H_INCLUDED
#define ENGINE_BOX_COLLIDER_H_INCLUDED

#include "define.h"
#include "ColliderBase3D.h"
#include "Maths/BoundingBox.h"
#include "Maths/Vector3.h"
#include "Raycast.h"

namespace gce {

DECLARE_COMPONENT_FAMILY( BoxCollider, ColliderBase3D, CollidersMask )
{
public:
    void Init() override;

    // Check if the ray intersects with BoxColliders
    bool RaycastCollider(Ray const& ray, RaycastHit & hitInfo, float32 maxDistance);

    Vector3f32 localScale{ 1.f,1.f,1.f };

    Vector<Vector3f32> localOBBPoints; //without Rot n Scale

    //bool m_dirty_offset;
    //bool m_dirty_scale; //#TODO dirty mask

protected:
    void UpdateColliderComponent( uint8 dirty ) override;
private:
    Box m_worldBox;

    friend class PhysicSystem;
};

}

#endif