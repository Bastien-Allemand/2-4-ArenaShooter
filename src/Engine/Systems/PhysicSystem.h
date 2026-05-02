#ifndef ENGINE_PHYSIC_SYSTEM_H_INCLUDED
#define ENGINE_PHYSIC_SYSTEM_H_INCLUDED

#include "define.h"
#include "Components/SphereCollider.h"
#include "Components/CircleCollider2D.h"
#include "Components/BoxCollider.h"
#include "Components/BoxCollider2D.h"
#include "Raycast.h"

namespace gce {


    class PhysicSystem final
    {
        PhysicSystem() = default;
        ~PhysicSystem() = default;

    public:
        bool IntersectRay(Ray const& ray, RaycastHit& hitInfo, float32 maxDistance);

    private:
        void HandlePhysicCollision3D();
        void HandlePhysicCollision2D();
        void HandlePhysicComponents(float32 deltaTime);
        void HandlePhysicCollisionResolve();
        void HandlePhysicCollisionResolve2D();

        ///Collisions
        //2D
        void Collide2DCircleCircle(CircleCollider2D& firstCircle, CircleCollider2D& secondCircle);
        void Collide2DCircleSquare(bool flag, CircleCollider2D& circleC, BoxCollider2D& boxC, Vector2f32 const& hitPoint);
        void Collide2DSquareSquare(BoxCollider2D& box1, BoxCollider2D& box2, Vector2f32 const& hitPoint, Vector2f32& const overlapVect);
        //3D
        void Collide3DSphereSphere(SphereCollider& firstSphere, SphereCollider& secondSphere, Vector3f32 const& hitPoint);
        void Collide3DSphereBox(bool flag, SphereCollider& sphereC, BoxCollider& boxC, Vector3f32 const& hitPoint);
        void Collide3DBoxBox(bool flag, BoxCollider& box1, BoxCollider& box2, Vector3f32 const& hitPoint, Vector3f32& overlapVect);

    private:
        Vector<CollisionPair> CollisionList;
        Vector<CollisionPair2D> CollisionList2D;

        friend class GameManager;
    };


}

#endif