#ifndef TEST_PHYSIC_CONTROLLER_HPP_INCLUDED
#define TEST_PHYSIC_CONTROLLER_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;
DECLARE_SCRIPT(PhysicsControllerScript, ScriptFlag::Update)

void Update()
{
    Vector3f32 currentPosition = m_pOwner->transform.GetWorldPosition();

    if (GetKey(Keyboard::Q))
    {
        Force leftForce;
        leftForce.direction = { -1,0,0 };
        leftForce.norm = 200;
        m_pOwner->GetComponent<PhysicComponent>()->AddForce(leftForce);
        //currentPosition.x -= 5 * GameManager::DeltaTime();
        //pGameObjet->m_movingDirection.x = -1.f;
    }
    if (GetKey(Keyboard::D))
    {
        Force rightForce;
        rightForce.direction = { 1,0,0 };
        rightForce.norm = 200;
        m_pOwner->GetComponent<PhysicComponent>()->AddForce(rightForce);
        //currentPosition.x += 5 * GameManager::DeltaTime();
        //pGameObjet->m_movingDirection.x = 1.f;

    }
    if (GetKey(Keyboard::Z))
    {
        Force upForce;
        upForce.direction = { 0,1,0 };
        upForce.norm = 200;
        m_pOwner->GetComponent<PhysicComponent>()->AddForce(upForce);
        //currentPosition.y += 5 * GameManager::DeltaTime();
        //pGameObjet->m_movingDirection.y = 1.f;
    }
    if (GetKey(Keyboard::S))
    {
        Force downForce;
        downForce.direction = { 0,-1,0 };
        downForce.norm = 200;
        m_pOwner->GetComponent<PhysicComponent>()->AddForce(downForce);
        //currentPosition.y -= 5 * GameManager::DeltaTime();
        //pGameObjet->m_movingDirection.y = -1.f;
    }

    if (IsGamepadConnected()) { // Input for controller here is for the Left Stick 

        Force currentForce;
        currentForce.direction = { GetLeftStick(0).x, GetLeftStick(0).y, 0 };
        currentForce.norm = 20000;
        m_pOwner->GetComponent<PhysicComponent>()->AddForce(currentForce);
        m_pOwner->GetComponent<PhysicComponent>()->SetAirDragCoefficient(1.f);
    }

    m_pOwner->transform.SetLocalPosition(currentPosition);

    wchar_t buffer[64];
    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"speed: %.2f\n", m_pOwner->transform.GetWorldPosition().z);
    OutputDebugString(buffer);
}

END_SCRIPT


BEGIN_TEST( PhysicController )

    GameManager::Create();
    Scene& scene = Scene::Create();

    D12PipelineObject pso(
        SHADERS.VERTEX,
        SHADERS.PIXEL,
        SHADERS.HULL,
        SHADERS.DOMAIN_,
        SHADERS.ROOT_SIGNATURE
    );
    GameObject& second = GameObject::Create(scene);
    second.AddScript<PhysicsControllerScript>();

    second.transform.SetLocalPosition({ 3, 10, 17 });
    second.transform.LocalScale({ 5, 5, 5 });
    second.transform.LocalRotate({XM_PIDIV4, -XM_PIDIV2, 0});
    MeshRenderer& meshRenderer2 = *second.AddComponent<MeshRenderer>();
    meshRenderer2.SetGeometry(SHAPES.SPHERE);
    SphereCollider& col = *second.AddComponent<SphereCollider>();
    col.isTrigger = false; // Set the collider as a trigger if tru then no collide no more
    PhysicComponent& physComp = *second.AddComponent<PhysicComponent>();

    physComp.SetGravityScale(0.f); // Apply gravity on the object 1.f is earth gravity


    Force newForce; // Add a force in a direction with a norm to define the strengh of it
    Vector3f32 dir{ -0.5f,1.f,0.f };
    newForce.direction = dir.Normalize();
    newForce.norm = 500.f;
    //physComp.AddForce(newForce);

    GameObject& first = GameObject::Create(scene);
    first.transform.SetLocalPosition({ -5, 0, 20 });
    first.transform.LocalScale({ 10,10,10 });
    MeshRenderer& meshRenderer = *first.AddComponent<MeshRenderer>();
    meshRenderer.SetGeometry(SHAPES.SPHERE);
    SphereCollider& col1 = *first.AddComponent<SphereCollider>();

    Camera& camera = *second.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.aspectRatio = 600.0f / 400.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };

    WindowParam windowParam;
    GameManager::Run(windowParam);

    GameManager::Destroy();

END_TEST_MAIN_FUNCTION( PhysicController )
END_TEST( PhysicController )


#endif