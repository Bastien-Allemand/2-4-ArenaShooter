#include "pch.h"
#include "PhysicSystem.h"
#include "Components/SphereCollider.h"
#include "Components/CircleCollider2D.h"
#include "Components/PhysicComponent.h"
#include "Components/BoxCollider.h"
#include "Components/BoxCollider2D.h"
#include "Maths/Physics.h"
#include "Maths/Vector3.h"
#include "Maths/MathsFunctions.hpp"
#include "GameTransform.h"
#include "GameObject.h"

namespace gce {
	/////////////////////////////////////
/// @brief Intersects a ray with all type of colliders (Sphere, Box)
/// @param ray Ray to intersect.
/// @param hitInfo Structure to store the hit information.
/// @param maxDistance Maximum distance to check for intersection.
/// @return true if Intersect
/////////////////////////////////////
	bool PhysicSystem::IntersectRay(Ray const& ray, RaycastHit& hitInfo, float32 maxDistance)
	{
		bool hasHit = false;
		float32 closestDistance = maxDistance;

		// BoxColliders
		for (BoxCollider* box : BoxCollider::s_list)
		{
			RaycastHit tempHit;
			if (box->RaycastCollider(ray, tempHit, closestDistance))
			{
				if (tempHit.distance < closestDistance)
				{
					closestDistance = tempHit.distance;
					hitInfo = tempHit;
					hasHit = true;
				}
			}
		}

		return hasHit;
	}

	void PhysicSystem::HandlePhysicCollision3D()
	{
		for (uint16 i = 0; i < SphereCollider::s_list.Size(); i++)
		{
			SphereCollider& sphere1 = *SphereCollider::s_list[i];
			if (sphere1.m_created == false || sphere1.IsActive() == false) continue;

			//Check SPHERE SPHERE

			for (uint16 j = i + 1; j < SphereCollider::s_list.Size(); j++)
			{
				SphereCollider& sphere2 = *SphereCollider::s_list[j];
				if (sphere2.m_created == false || sphere2.IsActive() == false) continue;

				bool flag = 0; //0 => sphere1 / 1 => sphere2
				bool hasPhys1 = sphere1.GetOwner().HasComponent<PhysicComponent>();
				bool hasPhys2 = sphere2.GetOwner().HasComponent<PhysicComponent>();
				if (!hasPhys1) flag = 1;

				if (!hasPhys1 && !hasPhys2)
				{
					if (!(sphere1.isTrigger || sphere2.isTrigger))
						continue;
				}

				CollideResult intersects = Physics::IntersectSphereSphere(sphere1.m_worldSphere, sphere2.m_worldSphere);

				bool wasColliding = false;
				for (uint32 IDs : sphere1.m_wasCollidingID)
				{
					if (IDs == sphere2.m_pOwner->m_id)
					{
						wasColliding = true;
						break;
					}
				}

				if (intersects.isColliding)
				{
					if (wasColliding)
					{
						//Still overlap
						sphere1.collisionStayEvent.Invoke(sphere2.m_pOwner);
						sphere2.collisionStayEvent.Invoke(sphere1.m_pOwner);
					}
					else
					{
						//Begin overlap
						sphere1.collisionEnterEvent.Invoke(sphere2.m_pOwner);
						sphere2.collisionEnterEvent.Invoke(sphere1.m_pOwner);
					}

					if (!(sphere1.isTrigger || sphere2.isTrigger))
					{
						if (hasPhys1 || hasPhys2)
						{
							CollisionPair pair;
							pair.type = CollisionType::SphereSphere;
							pair.A = &sphere1;
							pair.B = &sphere2;
							pair.physCompFlag = flag;
							pair.result = intersects;

							CollisionList.PushBack(pair);
						}
					}
				}
				else if (wasColliding)
				{
					//End overlap
					sphere1.collisionExitEvent.Invoke(sphere2.m_pOwner);
					sphere2.collisionExitEvent.Invoke(sphere1.m_pOwner);
				}

				if (wasColliding != intersects.isColliding)
				{
					if (!wasColliding && intersects.isColliding)
					{
						sphere1.m_wasCollidingID.PushBack(sphere2.m_pOwner->m_id);
						sphere2.m_wasCollidingID.PushBack(sphere1.m_pOwner->m_id);
					}
					else if (wasColliding && !intersects.isColliding)
					{
						for (size_t i = 0; i < sphere1.m_wasCollidingID.Size(); i++)
						{
							if (sphere1.m_wasCollidingID[i] == sphere2.m_pOwner->m_id)
							{
								sphere1.m_wasCollidingID.Erase(sphere1.m_wasCollidingID.begin() + i);
								break;
							}
						}
						for (size_t i = 0; i < sphere2.m_wasCollidingID.Size(); i++)
						{
							if (sphere2.m_wasCollidingID[i] == sphere1.m_pOwner->m_id)
							{
								sphere2.m_wasCollidingID.Erase(sphere2.m_wasCollidingID.begin() + i);
								break;
							}
						}
					}

				}
			}

			//Check SPHERE BOX

			for (uint16 j = 0; j < BoxCollider::s_list.Size(); j++)
			{
				BoxCollider& box1 = *BoxCollider::s_list[j];
				if (box1.m_created == false || box1.IsActive() == false) continue;

				bool flag = 0; //0 => sphere1 / 1 => box1
				bool hasPhysSphere = sphere1.GetOwner().HasComponent<PhysicComponent>();
				bool hasPhysBox = box1.GetOwner().HasComponent<PhysicComponent>();
				if (!hasPhysSphere) flag = 1;
				// Allow trigger-only pairs to pass so trigger events are emitted
				if (!hasPhysSphere && !hasPhysBox)
				{
					if (!(sphere1.isTrigger || box1.isTrigger))
						continue;
				}

				CollideResult intersects = Physics::IntersectSphereBox(sphere1.m_worldSphere, box1.m_worldBox);

				bool wasColliding = false;
				for (uint32 IDs : sphere1.m_wasCollidingID)
				{
					if (IDs == box1.m_pOwner->m_id)
					{
						wasColliding = true;
						break;
					}
				}

				if (intersects.isColliding)
				{
					if (wasColliding)
					{
						//Still overlap
						box1.collisionStayEvent.Invoke(sphere1.m_pOwner);
						sphere1.collisionStayEvent.Invoke(box1.m_pOwner);
					}
					else
					{
						//Begin overlap
						box1.collisionEnterEvent.Invoke(sphere1.m_pOwner);
						sphere1.collisionEnterEvent.Invoke(box1.m_pOwner);
					}

					if (!(sphere1.isTrigger || box1.isTrigger))
					{
						if (hasPhysSphere || hasPhysBox)
						{
							CollisionPair pair;
							pair.type = CollisionType::SphereBox;
							pair.A = &sphere1;
							pair.B = &box1;
							pair.physCompFlag = flag;
							pair.result = intersects;

							CollisionList.PushBack(pair);
						}
					}
				}
				else if (wasColliding)
				{
					//End overlap
					box1.collisionExitEvent.Invoke(sphere1.m_pOwner);
					sphere1.collisionExitEvent.Invoke(box1.m_pOwner);
				}

				if (wasColliding != intersects.isColliding)
				{
					if (!wasColliding && intersects.isColliding)
					{
						sphere1.m_wasCollidingID.PushBack(box1.m_pOwner->m_id);
						box1.m_wasCollidingID.PushBack(sphere1.m_pOwner->m_id);
					}
					else if (wasColliding && !intersects.isColliding)
					{
						for (size_t i = 0; i < sphere1.m_wasCollidingID.Size(); i++)
						{
							if (sphere1.m_wasCollidingID[i] == box1.m_pOwner->m_id)
							{
								sphere1.m_wasCollidingID.Erase(sphere1.m_wasCollidingID.begin() + i);
								break;
							}
						}
						for (size_t i = 0; i < box1.m_wasCollidingID.Size(); i++)
						{
							if (box1.m_wasCollidingID[i] == sphere1.m_pOwner->m_id)
							{
								box1.m_wasCollidingID.Erase(box1.m_wasCollidingID.begin() + i);
								break;
							}
						}
					}

				}
			}
		}

		//Check BOX BOX

		for (uint16 i = 0; i < BoxCollider::s_list.Size(); i++)
		{
			BoxCollider& box1 = *BoxCollider::s_list[i];
			if (box1.m_created == false || box1.IsActive() == false) continue;

			for (uint16 j = i + 1; j < BoxCollider::s_list.Size(); j++)
			{
				BoxCollider& box2 = *BoxCollider::s_list[j];
				if (box2.m_created == false || box2.IsActive() == false) continue;

				bool flag = 0; //0 => box1 / 1 => box2
				bool hasPhysA = box1.GetOwner().HasComponent<PhysicComponent>();
				bool hasPhysB = box2.GetOwner().HasComponent<PhysicComponent>();
				if (!hasPhysA) flag = 1;
				if (!hasPhysA && !hasPhysB)
				{
					if (!(box1.isTrigger || box2.isTrigger))
						continue;
				}

				CollideResult intersects = Physics::IntersectBoxBox(box1.m_worldBox, box2.m_worldBox);

				bool wasColliding = false;
				for (uint32 IDs : box1.m_wasCollidingID)
				{
					if (IDs == box2.m_pOwner->m_id)
					{
						wasColliding = true;
						break;
					}
				}

				if (intersects.isColliding)
				{
					if (wasColliding)
					{
						//Still overlap
						box1.collisionStayEvent.Invoke(box2.m_pOwner);
						box2.collisionStayEvent.Invoke(box1.m_pOwner);
					}
					else
					{
						//Begin overlap
						box1.collisionEnterEvent.Invoke(box2.m_pOwner);
						box2.collisionEnterEvent.Invoke(box1.m_pOwner);
					}

					if (!(box1.isTrigger || box2.isTrigger))
					{
						if (hasPhysA || hasPhysB)
						{
							CollisionPair pair;
							pair.type = CollisionType::BoxBox;
							pair.A = &box1;
							pair.B = &box2;
							pair.physCompFlag = flag;
							pair.result = intersects;

							CollisionList.PushBack(pair);
						}
					}
				}
				else if (wasColliding)
				{
					//End overlap
					box1.collisionExitEvent.Invoke(box2.m_pOwner);
					box2.collisionExitEvent.Invoke(box1.m_pOwner);
				}

				if (wasColliding != intersects.isColliding)
				{
					if (!wasColliding && intersects.isColliding)
					{
						box1.m_wasCollidingID.PushBack(box2.m_pOwner->m_id);
						box2.m_wasCollidingID.PushBack(box1.m_pOwner->m_id);
					}
					else if (wasColliding && !intersects.isColliding)
					{
						for (size_t i = 0; i < box1.m_wasCollidingID.Size(); i++)
						{
							if (box1.m_wasCollidingID[i] == box2.m_pOwner->m_id)
							{
								box1.m_wasCollidingID.Erase(box1.m_wasCollidingID.begin() + i);
								break;
							}
						}
						for (size_t i = 0; i < box2.m_wasCollidingID.Size(); i++)
						{
							if (box2.m_wasCollidingID[i] == box1.m_pOwner->m_id)
							{
								box2.m_wasCollidingID.Erase(box2.m_wasCollidingID.begin() + i);
								break;
							}
						}
					}

				}
			}
		}
	}

	void PhysicSystem::HandlePhysicCollision2D()
	{
		for (uint16 i = 0; i < CircleCollider2D::s_list.Size(); i++)
		{
			CircleCollider2D& circle1 = *CircleCollider2D::s_list[i];
			if (circle1.m_created == false || circle1.IsActive() == false) continue;

			// Check CIRCLE CIRCLE  

			for (uint16 j = i + 1; j < CircleCollider2D::s_list.Size(); j++)
			{
				CircleCollider2D& circle2 = *CircleCollider2D::s_list[j];
				if (circle2.m_created == false || circle2.IsActive() == false) continue;

				bool flag = 0; // 0 => circle1 / 1 => circle2  
				bool hasPhysA = circle1.GetOwner().HasComponent<PhysicComponent>();
				bool hasPhysB = circle2.GetOwner().HasComponent<PhysicComponent>();
				if (!hasPhysA) flag = 1;
				if (!hasPhysA && !hasPhysB)
				{
					if (!(circle1.isTrigger || circle2.isTrigger))
						continue;
				}

				CollideResult2D intersects = Physics::IntersectCircleCircle(circle1.m_worldCircle, circle2.m_worldCircle);

				bool wasColliding = false;
				for (uint32 IDs : circle1.m_wasCollidingID)
				{
					if (IDs == circle2.m_pOwner->m_id)
					{
						wasColliding = true;
						break;
					}
				}

				if (intersects.isColliding)
				{
					if (wasColliding)
					{
						// Still overlap
						circle1.collision2DStayEvent.Invoke(circle2.m_pOwner);
						circle2.collision2DStayEvent.Invoke(circle1.m_pOwner);
					}
					else
					{
						// Begin overlap
						circle1.collision2DEnterEvent.Invoke(circle2.m_pOwner);
						circle2.collision2DEnterEvent.Invoke(circle1.m_pOwner);
					}

					if (!(circle1.isTrigger || circle2.isTrigger))
					{
						if (hasPhysA || hasPhysB)
						{
							CollisionPair2D pair;
							pair.type = CollisionType::CircleCircle;
							pair.A = &circle1;
							pair.B = &circle2;
							pair.physCompFlag = flag;
							pair.result = intersects;

							CollisionList2D.PushBack(pair);
						}
					}
				}
				else if (wasColliding)
				{
					// End overlap  
					circle1.collision2DExitEvent.Invoke(circle2.m_pOwner);
					circle2.collision2DExitEvent.Invoke(circle1.m_pOwner);
				}

				if (wasColliding != intersects.isColliding)
				{
					if (!wasColliding && intersects.isColliding)
					{
						circle1.m_wasCollidingID.PushBack(circle2.m_pOwner->m_id);
						circle2.m_wasCollidingID.PushBack(circle1.m_pOwner->m_id);
					}
					else if (wasColliding && !intersects.isColliding)
					{
						for (size_t i = 0; i < circle1.m_wasCollidingID.Size(); i++)
						{
							if (circle1.m_wasCollidingID[i] == circle2.m_pOwner->m_id)
							{
								circle1.m_wasCollidingID.Erase(circle1.m_wasCollidingID.begin() + i);
								break;
							}
						}
						for (size_t i = 0; i < circle2.m_wasCollidingID.Size(); i++)
						{
							if (circle2.m_wasCollidingID[i] == circle1.m_pOwner->m_id)
							{
								circle2.m_wasCollidingID.Erase(circle2.m_wasCollidingID.begin() + i);
								break;
							}
						}
					}
				}
			}

			// Check CIRCLE SQUARE  

			for (uint16 j = 0; j < BoxCollider2D::s_list.Size(); j++)
			{
				BoxCollider2D& box1 = *BoxCollider2D::s_list[j];
				if (box1.m_created == false || box1.IsActive() == false) continue;

				bool flag = 0; // 0 => circle1 / 1 => box1  
				bool hasPhysCircle = circle1.GetOwner().HasComponent<PhysicComponent>();
				bool hasPhysBox = box1.GetOwner().HasComponent<PhysicComponent>();
				if (!hasPhysCircle) flag = 1;
				if (!hasPhysCircle && !hasPhysBox)
				{
					if (!(circle1.isTrigger || box1.isTrigger))
						continue;
				}

				CollideResult2D intersects = Physics::IntersectCircleSquare(circle1.m_worldCircle, box1.m_worldBox);

				bool wasColliding = false;
				for (uint32 IDs : circle1.m_wasCollidingID)
				{
					if (IDs == box1.m_pOwner->m_id)
					{
						wasColliding = true;
						break;
					}
				}

				if (intersects.isColliding)
				{
					if (wasColliding)
					{
						// Still overlap  
						circle1.collision2DStayEvent.Invoke(box1.m_pOwner);
						box1.collision2DStayEvent.Invoke(circle1.m_pOwner);
					}
					else
					{
						// Begin overlap  
						circle1.collision2DEnterEvent.Invoke(box1.m_pOwner);
						box1.collision2DEnterEvent.Invoke(circle1.m_pOwner);
					}

					if (!(circle1.isTrigger || box1.isTrigger))
					{
						if (hasPhysCircle || hasPhysBox)
						{
							CollisionPair2D pair;
							pair.type = CollisionType::CircleSquare;
							pair.A = &circle1;
							pair.B = &box1;
							pair.physCompFlag = flag;
							pair.result = intersects;

							CollisionList2D.PushBack(pair);
						}
					}
				}
				else if (wasColliding)
				{
					// End overlap  
					circle1.collision2DExitEvent.Invoke(box1.m_pOwner);
					box1.collision2DExitEvent.Invoke(circle1.m_pOwner);
				}

				if (wasColliding != intersects.isColliding)
				{
					if (!wasColliding && intersects.isColliding)
					{
						circle1.m_wasCollidingID.PushBack(box1.m_pOwner->m_id);
						box1.m_wasCollidingID.PushBack(circle1.m_pOwner->m_id);
					}
					else if (wasColliding && !intersects.isColliding)
					{
						for (size_t i = 0; i < circle1.m_wasCollidingID.Size(); i++)
						{
							if (circle1.m_wasCollidingID[i] == box1.m_pOwner->m_id)
							{
								circle1.m_wasCollidingID.Erase(circle1.m_wasCollidingID.begin() + i);
								break;
							}
						}
						for (size_t i = 0; i < box1.m_wasCollidingID.Size(); i++)
						{
							if (box1.m_wasCollidingID[i] == circle1.m_pOwner->m_id)
							{
								box1.m_wasCollidingID.Erase(box1.m_wasCollidingID.begin() + i);
								break;
							}
						}
					}
				}
			}
		}

		// Check SQUARE SQUARE  

		for (uint16 i = 0; i < BoxCollider2D::s_list.Size(); i++)
		{
			BoxCollider2D& box1 = *BoxCollider2D::s_list[i];
			if (box1.m_created == false || box1.IsActive() == false) continue;

			for (uint16 j = i + 1; j < BoxCollider2D::s_list.Size(); j++)
			{
				BoxCollider2D& box2 = *BoxCollider2D::s_list[j];
				if (box2.m_created == false || box2.IsActive() == false) continue;

				bool flag = 0; // 0 => box1 / 1 => box2  
				if (!box1.GetOwner().HasComponent<PhysicComponent>()) flag = 1;
				bool hasPhysA = box1.GetOwner().HasComponent<PhysicComponent>();
				bool hasPhysB = box2.GetOwner().HasComponent<PhysicComponent>();
				if (!hasPhysA) flag = 1;
				if (!hasPhysA && !hasPhysB)
				{
					if (!(box1.isTrigger || box2.isTrigger))
						continue;
				}

				CollideResult2D intersects = Physics::IntersectSquareSquare(box1.m_worldBox, box2.m_worldBox);

				bool wasColliding = false;
				for (uint32 IDs : box1.m_wasCollidingID)
				{
					if (IDs == box2.m_pOwner->m_id)
					{
						wasColliding = true;
						break;
					}
				}

				if (intersects.isColliding)
				{
					if (wasColliding)
					{
						// Still overlap  
						box1.collision2DStayEvent.Invoke(box2.m_pOwner);
						box2.collision2DStayEvent.Invoke(box1.m_pOwner);
					}
					else
					{
						// Begin overlap  
						box1.collision2DEnterEvent.Invoke(box2.m_pOwner);
						box2.collision2DEnterEvent.Invoke(box1.m_pOwner);
					}

					if (!(box1.isTrigger || box2.isTrigger))
					{
						if (hasPhysA || hasPhysB)
						{
							CollisionPair2D pair;
							pair.type = CollisionType::SquareSquare;
							pair.A = &box1;
							pair.B = &box2;
							pair.physCompFlag = flag;
							pair.result = intersects;

							CollisionList2D.PushBack(pair);
						}
					}
				}
				else if (wasColliding)
				{
					// End overlap  
					box1.collision2DExitEvent.Invoke(box2.m_pOwner);
					box2.collision2DExitEvent.Invoke(box1.m_pOwner);
				}

				if (wasColliding != intersects.isColliding)
				{
					if (!wasColliding && intersects.isColliding)
					{
						box1.m_wasCollidingID.PushBack(box2.m_pOwner->m_id);
						box2.m_wasCollidingID.PushBack(box1.m_pOwner->m_id);
					}
					else if (wasColliding && !intersects.isColliding)
					{
						for (size_t i = 0; i < box1.m_wasCollidingID.Size(); i++)
						{
							if (box1.m_wasCollidingID[i] == box2.m_pOwner->m_id)
							{
								box1.m_wasCollidingID.Erase(box1.m_wasCollidingID.begin() + i);
								break;
							}
						}
						for (size_t i = 0; i < box2.m_wasCollidingID.Size(); i++)
						{
							if (box2.m_wasCollidingID[i] == box1.m_pOwner->m_id)
							{
								box2.m_wasCollidingID.Erase(box2.m_wasCollidingID.begin() + i);
								break;
							}
						}
					}
				}
			}
		}
	}

	void PhysicSystem::HandlePhysicComponents(float32 deltaTime)
	{
		for (uint16 i = 0; i < PhysicComponent::s_list.Size(); i++)
		{
			PhysicComponent& physComp = *PhysicComponent::s_list[i];
			if (physComp.m_created == false || physComp.IsActive() == false) continue;

			Vector3f32 pos = physComp.GetOwner().transform.GetWorldPosition();
			Vector3f32 rot{ 0,0,0 };
			Vector2<Vector3f32> newData = physComp.UpdatePhysic(pos, rot, deltaTime);
			physComp.GetOwner().transform.SetWorldPosition(newData.x);
			physComp.GetOwner().transform.WorldRotate({ newData.y.x, newData.y.y, newData.y.z });
		}
	}

	void PhysicSystem::HandlePhysicCollisionResolve()
	{
		for (CollisionPair pair : CollisionList)
		{
			switch (pair.type)
			{
			case CollisionType::SphereSphere:
				pair.physCompFlag ? Collide3DSphereSphere(*reinterpret_cast<SphereCollider*>(pair.B), *reinterpret_cast<SphereCollider*>(pair.A), pair.result.hitPoint) : Collide3DSphereSphere(*reinterpret_cast<SphereCollider*>(pair.A), *reinterpret_cast<SphereCollider*>(pair.B), pair.result.hitPoint);
				break;
			case CollisionType::BoxBox:
				pair.physCompFlag ? Collide3DBoxBox(pair.physCompFlag, *reinterpret_cast<BoxCollider*>(pair.B), *reinterpret_cast<BoxCollider*>(pair.A), pair.result.hitPoint, pair.result.overlapVector) : Collide3DBoxBox(pair.physCompFlag, *reinterpret_cast<BoxCollider*>(pair.A), *reinterpret_cast<BoxCollider*>(pair.B), pair.result.hitPoint, pair.result.overlapVector);
				break;
			case CollisionType::SphereBox:
				Collide3DSphereBox(pair.physCompFlag, *reinterpret_cast<SphereCollider*>(pair.A), *reinterpret_cast<BoxCollider*>(pair.B), pair.result.hitPoint);
				break;

			default:
				break;
			}
		}
		CollisionList.Clear();
	}


#pragma region Collide2DRegion

	// SPHERE SPHERE

	void PhysicSystem::Collide2DCircleCircle(CircleCollider2D& firstCircle, CircleCollider2D& secondCircle)
	{
		Vector2f32 pos = firstCircle.m_worldCircle.center;
		Vector2f32 otherPos = secondCircle.m_worldCircle.center;
		Vector2f32 collisionDir = pos - otherPos;
		collisionDir.SelfNormalize();

		PhysicComponent* pPhysComp1 = firstCircle.GetOwner().GetComponent<PhysicComponent>();
		PhysicComponent* pPhysComp2 = secondCircle.GetOwner().GetComponent<PhysicComponent>();

		if (pPhysComp2)
		{
			float32 restitution = (pPhysComp1->m_bounciness + pPhysComp2->m_bounciness) * 0.5f;

			Vector3f32 newVel1 = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, pPhysComp2->m_mass,
				pPhysComp1->m_velocity, pPhysComp2->m_velocity,
				{ collisionDir.x, collisionDir.y, 0 }, restitution);

			Vector3f32 newVel2 = Physics::CalculateCollisionVelocity(
				pPhysComp2->m_mass, pPhysComp1->m_mass,
				pPhysComp2->m_velocity, pPhysComp1->m_velocity,
				{ -collisionDir.x, -collisionDir.y, -0 }, restitution);

			pPhysComp1->m_velocity = newVel1;
			pPhysComp2->m_velocity = newVel2;
		}
		else
		{
			pPhysComp1->m_velocity = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, 1e9f,
				pPhysComp1->m_velocity, { 0.f,0.f,0.f },
				{ collisionDir.x, collisionDir.y, 0 }, pPhysComp1->m_bounciness);
		}

		//Anti overlap system
		float32 dist = (pos - otherPos).Norm();
		float32 overlap = (firstCircle.m_worldCircle.radius + secondCircle.m_worldCircle.radius) - dist;
		if (overlap > 0.f)
		{
			Vector2f32 direction = collisionDir * overlap;

			if (pPhysComp2)
			{
				Vector2f32 newPos = pos + direction;
				firstCircle.GetOwner().transform.SetWorldPosition({ newPos.x, newPos.y, 0.0f });
				newPos = otherPos - direction;
				secondCircle.GetOwner().transform.SetWorldPosition({ newPos.x, newPos.y, 0.0f });
			}
			else
			{
				Vector2f32 newPos = pos + direction;
				firstCircle.GetOwner().transform.SetWorldPosition({ newPos.x, newPos.y, 0.0f });
			}
		}
	}

	// CIRCLE SQUARE

	void PhysicSystem::Collide2DCircleSquare(bool flag, CircleCollider2D& circleC, BoxCollider2D& boxC, Vector2f32 const& hitPoint)
	{
		// R cup ration des centres
		Vector2f32 circleCenter = circleC.m_worldCircle.center;
		Vector2f32 boxCenter = boxC.m_worldBox.center;

		// Calcul de la direction collision (du centre du cercle vers le point de contact)
		Vector2f32 collisionDir = circleCenter - hitPoint;
		float32 dist = collisionDir.Norm();
		if (dist > 1e-6f)
			collisionDir /= dist;
		else
			collisionDir = { 1.f, 0.f }; // Valeur par d faut si superpos s

		// R cup ration des composants physiques
		PhysicComponent* pPhysComp1;
		PhysicComponent* pPhysComp2;
		int16 flagModifier = 1;

		if (flag) {
			pPhysComp1 = boxC.GetOwner().GetComponent<PhysicComponent>();
			pPhysComp2 = circleC.GetOwner().GetComponent<PhysicComponent>();
			flagModifier = -1;
		}
		else {
			pPhysComp1 = circleC.GetOwner().GetComponent<PhysicComponent>();
			pPhysComp2 = boxC.GetOwner().GetComponent<PhysicComponent>();
		}

		// Calcul des nouvelles vitesses apr s collision
		if (pPhysComp2) {
			float32 restitution = (pPhysComp1->m_bounciness + pPhysComp2->m_bounciness) * 0.5f;

			Vector3f32 newVel1 = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, pPhysComp2->m_mass,
				pPhysComp1->m_velocity, pPhysComp2->m_velocity,
				{ collisionDir.x, collisionDir.y, 0 }, restitution);

			Vector3f32 newVel2 = Physics::CalculateCollisionVelocity(
				pPhysComp2->m_mass, pPhysComp1->m_mass,
				pPhysComp2->m_velocity, pPhysComp1->m_velocity,
				{ -collisionDir.x, -collisionDir.y, 0 }, restitution);

			pPhysComp1->m_velocity = newVel1;
			pPhysComp2->m_velocity = newVel2;
		}
		else {
			pPhysComp1->m_velocity = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, 1e9f,
				pPhysComp1->m_velocity, { 0.f,0.f,0.f },
				{ collisionDir.x, collisionDir.y, 0 }, pPhysComp1->m_bounciness);
		}

		// Correction d'overlap (anti-enchev trement)
		float32 overlap = circleC.m_worldCircle.radius - (hitPoint - circleCenter).Norm();
		if (overlap > 0.f) {
			Vector2f32 correction = collisionDir * (overlap * static_cast<float32>(flagModifier));

			if (flag) {
				// On d place le carr  si c'est lui qui est dynamique
				Vector2f32 newPos = boxC.m_worldBox.center + correction;
				boxC.GetOwner().transform.SetWorldPosition({ newPos.x, newPos.y, 0.0f });
			}
			else {
				// Sinon, on d place le cercle
				Vector2f32 newPos = circleCenter + correction;
				circleC.GetOwner().transform.SetWorldPosition({ newPos.x, newPos.y, 0.0f });
			}
		}
	}


	// SQUARE SQUARE

	void PhysicSystem::Collide2DSquareSquare(BoxCollider2D& box1, BoxCollider2D& box2, Vector2f32 const& hitPoint, Vector2f32& const overlapVect)
	{
		// Calculer le centre r el des carr s
		Vector2f32 offsetA = (box1.m_worldBox.max + box1.m_worldBox.min) * 0.5f;
		Vector2f32 offsetB = (box2.m_worldBox.max + box2.m_worldBox.min) * 0.5f;

		Vector2f32 pos = box1.m_worldBox.center +
			box1.m_worldBox.axisX * offsetA.x +
			box1.m_worldBox.axisY * offsetA.y;

		Vector2f32 otherPos = box2.m_worldBox.center +
			box2.m_worldBox.axisX * offsetB.x +
			box2.m_worldBox.axisY * offsetB.y;

		Vector2f32 collisionDir1 = hitPoint - pos;
		collisionDir1.SelfNormalize();
		Vector2f32 collisionDir2 = hitPoint - otherPos;
		collisionDir2.SelfNormalize();

		// D terminer la direction de collision dominante (axe X ou Y)
		Vector2f32 localHitDir = hitPoint - pos;
		float32 absX = Abs(localHitDir.x);
		float32 absY = Abs(localHitDir.y);

		if (absX > absY)
		{
			if (localHitDir.y > 0)
			{
				collisionDir1 = { 0.f, -1.f };
				collisionDir2 = { 0.f, 1.f };
			}
			else
			{
				collisionDir1 = { 0.f, 1.f };
				collisionDir2 = { 0.f, -1.f };
			}
		}
		else
		{
			if (localHitDir.x > 0)
			{
				collisionDir1 = { -1.f, 0.f };
				collisionDir2 = { 1.f, 0.f };
			}
			else
			{
				collisionDir1 = { 1.f, 0.f };
				collisionDir2 = { -1.f, 0.f };
			}
		}

		// Resolution de la collision
		PhysicComponent* pPhysComp1 = box1.GetOwner().GetComponent<PhysicComponent>();
		PhysicComponent* pPhysComp2 = box2.GetOwner().GetComponent<PhysicComponent>();

		if (pPhysComp2)
		{
			float32 restitution = (pPhysComp1->m_bounciness + pPhysComp2->m_bounciness) * 0.5f;

			Vector3f32 newVel1 = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, pPhysComp2->m_mass,
				pPhysComp1->m_velocity, pPhysComp2->m_velocity,
				{ collisionDir1.x, collisionDir1.y, 0 }, restitution);

			Vector3f32 newVel2 = Physics::CalculateCollisionVelocity(
				pPhysComp2->m_mass, pPhysComp1->m_mass,
				pPhysComp2->m_velocity, pPhysComp1->m_velocity,
				{ collisionDir2.x, collisionDir2.y, 0 }, restitution);

			pPhysComp1->m_velocity = newVel1;
			pPhysComp2->m_velocity = newVel2;
		}
		else
		{
			pPhysComp1->m_velocity = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, 1e9f,
				pPhysComp1->m_velocity, { 0.f,0.f,0.f },
				{ collisionDir1.x, collisionDir1.y, 0 }, pPhysComp1->m_bounciness);
		}

		if (overlapVect.Norm() > 10.f)
		{
			Vector2f32 delta = box2.m_worldBox.aabb.center - box1.m_worldBox.aabb.center;
			Vector2f32 overlap;

			Vector2f32 totalHalfSize = (box1.m_worldBox.aabb.max - box1.m_worldBox.aabb.min) * 0.5f +
				(box2.m_worldBox.aabb.max - box2.m_worldBox.aabb.min) * 0.5f;
			Vector2f32 absDelta = {
				Abs(delta.x),
				Abs(delta.y)
			};

			overlap.x = totalHalfSize.x - absDelta.x;
			overlap.y = totalHalfSize.y - absDelta.y;

			float32 minOverlap = overlap.x;
			Vector2f32 resolveDir = { 1.f, 0.f };

			if (overlap.y < minOverlap)
			{
				minOverlap = overlap.y;
				resolveDir = { 0.f, 1.f };
			}

			if (resolveDir.DotProduct(delta) > 0)
				resolveDir = -resolveDir;

			if (pPhysComp2)
			{
				box1.GetOwner().transform.SetWorldPosition({ box1.m_worldBox.center.x + resolveDir.x * minOverlap * 0.5f,
											   box1.m_worldBox.center.y + resolveDir.y * minOverlap * 0.5f,
											   0.0f });
				box2.GetOwner().transform.SetWorldPosition({ box2.m_worldBox.center.x - resolveDir.x * minOverlap * 0.5f,
											   box2.m_worldBox.center.y - resolveDir.y * minOverlap * 0.5f,
											   0.0f });
			}
			else
			{
				box1.GetOwner().transform.SetWorldPosition({ box1.m_worldBox.center.x + resolveDir.x * minOverlap,
											   box1.m_worldBox.center.y + resolveDir.y * minOverlap,
											   0.0f });
			}
		}
		else
		{
			Vector2f32 direction = box1.m_worldBox.center - box2.m_worldBox.center;
			Vector2f32 overlapV = overlapVect;
			if (direction.DotProduct(overlapV) <= 0.f) overlapV = -overlapV;

			if (pPhysComp2)
			{
				box1.GetOwner().transform.SetWorldPosition({ box1.m_worldBox.center.x + overlapV.x * 0.5f,
											   box1.m_worldBox.center.y + overlapV.y * 0.5f,
											   0.0f });
				box2.GetOwner().transform.SetWorldPosition({ box2.m_worldBox.center.x - overlapV.x * 0.5f,
											   box2.m_worldBox.center.y - overlapV.y * 0.5f,
											   0.0f });
			}
			else
			{
				box1.GetOwner().transform.SetWorldPosition({ box1.m_worldBox.center.x + overlapV.x,
								   box1.m_worldBox.center.y + overlapV.y,
								   0.0f });
			}
		}
	}


	void PhysicSystem::HandlePhysicCollisionResolve2D()
	{
		for (CollisionPair2D pair : CollisionList2D)
		{
			switch (pair.type)
			{
			case CollisionType::CircleCircle:
				pair.physCompFlag ? Collide2DCircleCircle(*reinterpret_cast<CircleCollider2D*>(pair.B), *reinterpret_cast<CircleCollider2D*>(pair.A)) : Collide2DCircleCircle(*reinterpret_cast<CircleCollider2D*>(pair.A), *reinterpret_cast<CircleCollider2D*>(pair.B));
				break;
			case CollisionType::SquareSquare:
				pair.physCompFlag ? Collide2DSquareSquare(*reinterpret_cast<BoxCollider2D*>(pair.B), *reinterpret_cast<BoxCollider2D*>(pair.A), pair.result.hitPoint, pair.result.overlapVector) : Collide2DSquareSquare(*reinterpret_cast<BoxCollider2D*>(pair.A), *reinterpret_cast<BoxCollider2D*>(pair.B), pair.result.hitPoint, pair.result.overlapVector);
				break;
			case CollisionType::CircleSquare:
				Collide2DCircleSquare(pair.physCompFlag, *reinterpret_cast<CircleCollider2D*>(pair.A), *reinterpret_cast<BoxCollider2D*>(pair.B), pair.result.hitPoint);
				break;

			default:
				break;
			}
		}
		CollisionList2D.Clear();
	}

#pragma endregion


#pragma region Collide3DRegion

	// SPHERE SPHERE
	void PhysicSystem::Collide3DSphereSphere(SphereCollider& firstSphere, SphereCollider& secondSphere, Vector3f32 const& hitPoint)
	{
		Vector3f32 pos = firstSphere.m_worldSphere.center;
		Vector3f32 otherPos = secondSphere.m_worldSphere.center;
		Vector3f32 collisionDir = pos - otherPos;
		collisionDir.SelfNormalize();

		PhysicComponent* pPhysComp1 = firstSphere.GetOwner().GetComponent<PhysicComponent>();
		PhysicComponent* pPhysComp2 = secondSphere.GetOwner().GetComponent<PhysicComponent>();

		if (pPhysComp2)
		{
			float32 restitution = (pPhysComp1->m_bounciness + pPhysComp2->m_bounciness) * 0.5f;

			Vector3f32 newVel1 = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, pPhysComp2->m_mass,
				pPhysComp1->m_velocity, pPhysComp2->m_velocity,
				collisionDir, restitution);

			Vector3f32 newVel2 = Physics::CalculateCollisionVelocity(
				pPhysComp2->m_mass, pPhysComp1->m_mass,
				pPhysComp2->m_velocity, pPhysComp1->m_velocity,
				-collisionDir, restitution);

			pPhysComp1->m_velocity = newVel1;
			pPhysComp2->m_velocity = newVel2;
		}
		else
		{
			pPhysComp1->m_velocity = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, 1e9f,
				pPhysComp1->m_velocity, { 0.f,0.f,0.f },
				collisionDir, pPhysComp1->m_bounciness);
		}

		//Anti overlap system

		float32 dist = (pos - otherPos).Norm();
		float32 overlap = (firstSphere.m_worldSphere.ray + secondSphere.m_worldSphere.ray) - dist;
		if (IsMore(overlap, 0.f, 1))
		{
			Vector3f32 direction = collisionDir * overlap;

			if (pPhysComp2)
			{
				firstSphere.GetOwner().transform.SetWorldPosition(pos + direction * 0.5f);
				secondSphere.GetOwner().transform.SetWorldPosition(otherPos + -direction * 0.5f);
			}
			else
			{
				firstSphere.GetOwner().transform.SetWorldPosition(pos + direction);
			}
		}
	}

	// SPHERE BOX
	void PhysicSystem::Collide3DSphereBox(bool flag, SphereCollider& sphereC, BoxCollider& boxC, Vector3f32 const& hitPoint)
	{
		Vector3f32 pos = sphereC.m_worldSphere.center;
		Vector3f32 otherPos = boxC.m_worldBox.center;

		Vector3f32 collisionDir = pos - hitPoint;
		collisionDir.SelfNormalize();

		PhysicComponent* pPhysComp1;
		PhysicComponent* pPhysComp2;
		int16 flagModifier = 1;

		if (!flag)
		{
			pPhysComp1 = sphereC.GetOwner().GetComponent<PhysicComponent>();
			pPhysComp2 = boxC.GetOwner().GetComponent<PhysicComponent>();
		}
		else
		{
			pPhysComp1 = boxC.GetOwner().GetComponent<PhysicComponent>();
			pPhysComp2 = sphereC.GetOwner().GetComponent<PhysicComponent>();
			flagModifier = -1;
		}

		if (pPhysComp2)
		{
			float32 restitution = (pPhysComp1->m_bounciness + pPhysComp2->m_bounciness) * 0.5f;

			Vector3f32 newVel1 = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, pPhysComp2->m_mass,
				pPhysComp1->m_velocity, pPhysComp2->m_velocity,
				collisionDir * flagModifier, restitution);

			Vector3f32 newVel2 = Physics::CalculateCollisionVelocity(
				pPhysComp2->m_mass, pPhysComp1->m_mass,
				pPhysComp2->m_velocity, pPhysComp1->m_velocity,
				-collisionDir * flagModifier, restitution);

			pPhysComp1->m_velocity = newVel1;
			pPhysComp2->m_velocity = newVel2;
		}
		else
		{
			pPhysComp1->m_velocity = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, 1e9f,
				pPhysComp1->m_velocity, { 0.f,0.f,0.f },
				collisionDir * flagModifier, pPhysComp1->m_bounciness);
		}

		//Anti overlap system

		float32 dist1 = (hitPoint - pos).Norm();
		float32 overlap = sphereC.m_worldSphere.ray - dist1;

		if (IsMore(overlap, 0.f, 1))
		{
			Vector3f32 correction = collisionDir.Normalize() * flagModifier * overlap;

			if (!flag)
			{
				Vector3f32 newPos = pos + correction;
				sphereC.GetOwner().transform.SetWorldPosition(newPos);
			}
			else
			{
				Vector3f32 newPos = otherPos + correction;
				boxC.GetOwner().transform.SetWorldPosition(newPos);
			}
		}
	}

	// BOX BOX 
	void PhysicSystem::Collide3DBoxBox(bool flag, BoxCollider& box1, BoxCollider& box2, Vector3f32 const& hitPoint, Vector3f32& overlapVect)
	{
		//Calculate real center of the boxes
		Vector3f32 offsetA = (box1.m_worldBox.max + box1.m_worldBox.min) * 0.5f;
		Vector3f32 offsetB = (box2.m_worldBox.max + box2.m_worldBox.min) * 0.5f;

		Vector3f32 pos = box1.m_worldBox.center +
			box1.m_worldBox.axisX * offsetA.x +
			box1.m_worldBox.axisY * offsetA.y +
			box1.m_worldBox.axisZ * offsetA.z;

		Vector3f32 otherPos = box2.m_worldBox.center +
			box2.m_worldBox.axisX * offsetB.x +
			box2.m_worldBox.axisY * offsetB.y +
			box2.m_worldBox.axisZ * offsetB.z;

		Vector3f32 collisionDir = overlapVect.Normalize();

		// Solve Collision

		PhysicComponent* pPhysComp1 = box1.GetOwner().GetComponent<PhysicComponent>();
		PhysicComponent* pPhysComp2 = box2.GetOwner().GetComponent<PhysicComponent>();

		if (pPhysComp2)
		{
			float32 restitution = (pPhysComp1->m_bounciness + pPhysComp2->m_bounciness) * 0.5f;

			Vector3f32 newVel1 = Physics::CalculateCollisionVelocity(
				pPhysComp1->m_mass, pPhysComp2->m_mass,
				pPhysComp1->m_velocity, pPhysComp2->m_velocity,
				-collisionDir, restitution);

			Vector3f32 newVel2 = Physics::CalculateCollisionVelocity(
				pPhysComp2->m_mass, pPhysComp1->m_mass,
				pPhysComp2->m_velocity, pPhysComp1->m_velocity,
				collisionDir, restitution);

			pPhysComp1->m_velocity = newVel1;
			pPhysComp2->m_velocity = newVel2;
		}
		else
		{
			if (flag)
			{
				pPhysComp1->m_velocity = Physics::CalculateCollisionVelocity(
					pPhysComp1->m_mass, 1e9f,
					pPhysComp1->m_velocity, { 0.f,0.f,0.f },
					collisionDir, pPhysComp1->m_bounciness);
			}
			else
			{
				pPhysComp1->m_velocity = Physics::CalculateCollisionVelocity(
					pPhysComp1->m_mass, 1e9f,
					pPhysComp1->m_velocity, { 0.f,0.f,0.f },
					-collisionDir, pPhysComp1->m_bounciness);
			}
		}

		//Anti overlap system

		if (IsMore(overlapVect.Norm(), 0.f, 1))
		{
			Vector3f32 direction = box1.m_worldBox.center - box2.m_worldBox.center;
			if (IsLessEqual(direction.DotProduct(overlapVect), 0.f, 1)) overlapVect = -overlapVect;

			if (pPhysComp2)
			{
				box1.GetOwner().transform.SetWorldPosition(box1.GetOwner().transform.GetWorldPosition() + overlapVect * 0.5f);
				box2.GetOwner().transform.SetWorldPosition(box2.GetOwner().transform.GetWorldPosition() - overlapVect * 0.5f);
			}
			else
			{
				box1.GetOwner().transform.SetWorldPosition(box1.GetOwner().transform.GetWorldPosition() + overlapVect);
			}
		}
	}

#pragma endregion
}