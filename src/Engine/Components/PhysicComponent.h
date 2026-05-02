#ifndef ENGINE_PHYSIC_COMPONENT_H_INCLUDED
#define ENGINE_PHYSIC_COMPONENT_H_INCLUDED

#include "define.h"
#include "Base.h"
#include "Maths/MathsFunctions.hpp"
#include "Maths/Physics.h"
//#include "Core/Containers/Vector.hpp"

namespace gce {

DECLARE_COMPONENT( PhysicComponent, ComponentBase )
{
public:

    PhysicComponent();
    PhysicComponent(uint16 maxNbForces);
    ~PhysicComponent() = default;

    void InitializeLists();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Add a force constantly applied to the object where the applicationPoint does not apply, example = thruster
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void AddConstantForce(Force& newForce);
    //////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Add a force applied to the object where the applicationPoint does not apply
    //////////////////////////////////////////////////////////////////////////////////////////
    void AddForce(Force& newForce);
    /////////////////////////////////////////////////////////////////////////
    /// @brief Add a force applied to the object with an applicationPoint
    /////////////////////////////////////////////////////////////////////////
    void AddForceWithPosition(Force& newForce);
    ////////////////////////////////////////////////////////////////////////////////////
    /// @brief Add a force constantly applied to the object with an applicationPoint
    ////////////////////////////////////////////////////////////////////////////////////
    void AddConstantForceWithPosition(Force& newForce);
    ///////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Add a torque applied to the object where the applicationPoint does not apply
    ///////////////////////////////////////////////////////////////////////////////////////////
    void AddTorque(Vector3f32& torque);

    void SetGravityScale(float32 newGravityScale);
    void SetAirDragCoefficient(float32 newDragCoefficient);
    void SetStaticDragCoefficient(float32 newDragCoefficient);
    void SetDynamicDragCoefficient(float32 newDragCoefficient);
    void SetMass(float32 newMass);
    void SetBounciness(float32 newBounciness);
    void SetVelocity(Vector3f32 newVelocity);
    void SetRelativeCenterOfMass(Vector3f32 newCenterOfMass);

    [[nodiscard]] float32 GetGravityScale() const;
    [[nodiscard]] Vector3f32 GetVelocity() const;
    [[nodiscard]] Vector3f32 GetAngularVelocity() const;

protected:
    Vector2<Vector3f32> UpdatePhysic(Vector3f32& previousPosition, Vector3f32& previousRotation, float32 deltaTime);

    //=== Settable members ===

    uint16 m_maxNbForces = 10;

    float32 m_gravityScale = 1.0f;
    float32 m_airDragCoefficient = 0.2f;
    float32 m_staticDragCoefficient = 5.f;
    float32 m_dynamicDragCoefficient = 0.2f;
    //float32 m_epsilonDrag = 0.1f;
    float32 m_angularFrictionCoeff = 0.5f;

    float32 m_bounciness = 0.7f;
    float32 m_mass = 20.f;

    Vector3f32 m_centerOfMass{ 0,0,0 };
    Vector3f32 m_velocity{ 0,0,0 };
    Vector3f32 m_angularVelocity{ 0,0,0 };

    float32 m_momentOfInertia = 2.f;     //TODO adapter en matrice selon la geometrie

    //=== Other members ===

    Vector3f32 m_staticForce{ 0,0,0 };

    Vector<Force> m_constForceList;
    Vector<Force> m_forceList;
    Vector<Force> m_appliedTorqueForceList;
    Vector<Vector3f32> m_torqueList;

    friend class PhysicSystem;
};


}

#include "PhysicComponent.inl"

#endif
