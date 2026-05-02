#ifndef ENGINE_PHYSIC_COMPONENT_INL_INCLUDED
#define ENGINE_PHYSIC_COMPONENT_INL_INCLUDED

namespace gce {


inline void PhysicComponent::SetGravityScale(float32 newGravityScale) {m_gravityScale = newGravityScale;}
inline void PhysicComponent::SetAirDragCoefficient(float32 newAirDragCoefficient) {m_airDragCoefficient = newAirDragCoefficient;}
inline void PhysicComponent::SetStaticDragCoefficient(float32 newStaticDragCoefficient) {m_staticDragCoefficient = newStaticDragCoefficient;}
inline void PhysicComponent::SetDynamicDragCoefficient(float32 newDynamicDragCoefficient) {m_dynamicDragCoefficient = newDynamicDragCoefficient;}
inline void PhysicComponent::SetMass(float32 newMass) { if(newMass > EPSILON) m_mass = newMass; }
inline void PhysicComponent::SetBounciness(float32 newBounciness) {m_bounciness = newBounciness;}
inline void PhysicComponent::SetVelocity(Vector3f32 newVelocity){m_velocity = newVelocity;}
inline void PhysicComponent::SetRelativeCenterOfMass(Vector3f32 newCenterOfMass){m_centerOfMass = newCenterOfMass;}
inline float32 PhysicComponent::GetGravityScale() const {return m_gravityScale;}
inline Vector3f32 PhysicComponent::GetVelocity() const {return m_velocity;}
inline Vector3f32 PhysicComponent::GetAngularVelocity() const {return m_angularVelocity;}


}

#endif