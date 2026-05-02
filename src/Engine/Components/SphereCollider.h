#ifndef ENGINE_SPHERE_COLLIDER_H_INCLUDED
#define ENGINE_SPHERE_COLLIDER_H_INCLUDED

#include "define.h"
#include "ColliderBase3D.h"
#include "Maths/Sphere.h"

namespace gce {

DECLARE_COMPONENT_FAMILY( SphereCollider, ColliderBase3D, CollidersMask ) 
{
public:
    void Init() override;

    float32 localScale = 1.f;
    float32 localRay = .5f;



    //bool m_dirty_offset;
    //bool m_dirty_scale; //#TODO dirty mask

protected:
    void UpdateColliderComponent( uint8 dirty ) override;
private:
    Sphere m_worldSphere;

    friend class PhysicSystem;
};

}

#endif