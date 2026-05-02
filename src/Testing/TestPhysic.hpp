#ifndef TEST_PHYSIC_HPP_INCLUDED
#define TEST_PHYSIC_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include <InputsMethods.h>
#include "define.h"
using namespace gce;

BEGIN_SUBTEST( Physic, Gravity )

    GameManager::Create();
    Scene& scene = Scene::Create();


    GameObject& first = GameObject::Create(scene);
    first.transform.SetLocalPosition({ 0.f, 6.f, 10.f });
    MeshRenderer& meshRenderer = *first.AddComponent<MeshRenderer>();
    meshRenderer.SetGeometry(SHAPES.CUBE);
    SphereCollider& col1 = *first.AddComponent<SphereCollider>();
    PhysicComponent& physComp1 = *first.AddComponent<PhysicComponent>();

    GameObject& second = GameObject::Create(scene);
    second.transform.SetLocalPosition({ 3.f, 6.f, 10.f });
    MeshRenderer& meshRenderer2 = *second.AddComponent<MeshRenderer>();
    meshRenderer2.SetGeometry(SHAPES.CUBE);
    SphereCollider& col2 = *second.AddComponent<SphereCollider>();
    PhysicComponent& physComp2 = *second.AddComponent<PhysicComponent>();
    physComp2.SetMass(200.f);

    GameObject& third = GameObject::Create(scene);
    third.transform.SetLocalPosition({ 6.f, 6.f, 10.f });
    MeshRenderer& meshRenderer3 = *third.AddComponent<MeshRenderer>();
    meshRenderer3.SetGeometry(SHAPES.CUBE);
    SphereCollider& col3 = *third.AddComponent<SphereCollider>();
    PhysicComponent& physComp3 = *third.AddComponent<PhysicComponent>();
    physComp3.SetGravityScale(1.6f);

    WindowParam windowParam;
    GameManager::Run(windowParam);

    GameManager::Destroy();

END_SUBTEST( Gravity )


BEGIN_SUBTEST( Physic, Forces )

    GameManager::Create();
    Scene& scene = Scene::Create();
    
    GameObject& cameraObject = GameObject::Create( scene );
    cameraObject.transform.LocalTranslate( { 0, 10, -10 } );
    cameraObject.transform.LocalRotate( { 0.35f, 0, 0 } );
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.aspectRatio = 600.0f / 400.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };

    GameObject& first = GameObject::Create(scene);
    first.transform.SetLocalPosition({ 0.f, 6.f, 10.f });
    MeshRenderer& meshRenderer = *first.AddComponent<MeshRenderer>();
    meshRenderer.SetGeometry(SHAPES.CUBE);
    SphereCollider& col1 = *first.AddComponent<SphereCollider>();
    PhysicComponent& physComp1 = *first.AddComponent<PhysicComponent>();
    Force newForce;
    Vector3f32 dir{ -0.5f,1.f,0.f };
    newForce.direction = dir.Normalize();
    newForce.norm = 500.f;
    physComp1.AddForce(newForce);

    GameObject& second = GameObject::Create(scene);
    second.transform.SetLocalPosition({ 3.f, 6.f, 20.f });
    MeshRenderer& meshRenderer2 = *second.AddComponent<MeshRenderer>();
    meshRenderer2.SetGeometry(SHAPES.CUBE);
    SphereCollider& col2 = *second.AddComponent<SphereCollider>();
    PhysicComponent& physComp2 = *second.AddComponent<PhysicComponent>();
    Force newForce2;
    Vector3f32 dir2{ 0.5f,1.f,0.f };
    newForce2.direction = dir2.Normalize();
    newForce2.norm = 500.f;
    physComp2.AddForce(newForce2);


    GameObject& third = GameObject::Create(scene);
    third.transform.SetLocalPosition({ 6.f, 6.f, 20.f });
    MeshRenderer& meshRenderer3 = *third.AddComponent<MeshRenderer>();
    meshRenderer3.SetGeometry(SHAPES.CUBE);
    SphereCollider& col3 = *third.AddComponent<SphereCollider>();
    PhysicComponent& physComp3 = *third.AddComponent<PhysicComponent>();
    Force newForce3;
    Vector3f32 dir3{ 0.5f,1.f,0.f };
    newForce3.direction = dir3.Normalize();
    newForce3.norm = 1000.f;
    physComp3.AddForce(newForce3);

    GameObject& fourth = GameObject::Create(scene);
    fourth.transform.SetLocalPosition({ -3.f, 0.f, 10.f });
    MeshRenderer& meshRenderer4 = *fourth.AddComponent<MeshRenderer>();
    meshRenderer4.SetGeometry(SHAPES.CUBE);
    SphereCollider& col4 = *fourth.AddComponent<SphereCollider>();
    PhysicComponent& physComp4 = *fourth.AddComponent<PhysicComponent>();
    physComp4.SetGravityScale(1.6f);
    Force newForce4;
    Vector3f32 dir4{ 0.f,1.f,0.f };
    newForce4.direction = dir4.Normalize();
    newForce4.norm = 350.f;
    physComp4.AddConstantForce(newForce4);

    WindowParam windowParam;
    GameManager::Run(windowParam);

    GameManager::Destroy();

END_SUBTEST( Forces )


BEGIN_SUBTEST( Physic, StaticCollision )

    GameManager::Create();
    Scene& scene = Scene::Create();
    
    GameObject& cameraObject = GameObject::Create( scene );
    cameraObject.transform.LocalTranslate( { 0, 10, -10 } );
    cameraObject.transform.LocalRotate( { 0.35f, 0, 0 } );
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.aspectRatio = 600.0f / 400.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };

    GameObject& ground = GameObject::Create(scene);
    ground.transform.SetLocalPosition({ 6.f, -200.f, 20.f });
    ground.transform.LocalScale({ 400.f,400.f,400.f });
    MeshRenderer& meshRenderer = *ground.AddComponent<MeshRenderer>();
    meshRenderer.SetGeometry(SHAPES.CUBE);
    SphereCollider& col1 = *ground.AddComponent<SphereCollider>();


    GameObject& second = GameObject::Create(scene);
    second.transform.SetLocalPosition({ 9.f, 6.f, 20.f });
    second.transform.LocalScale({ 1.f, 1.f, 1.f });
    MeshRenderer& meshRenderer2 = *second.AddComponent<MeshRenderer>();
    meshRenderer2.SetGeometry(SHAPES.CUBE);
    SphereCollider& col = *second.AddComponent<SphereCollider>();
    PhysicComponent& physComp = *second.AddComponent<PhysicComponent>();

    GameObject& third = GameObject::Create(scene);
    third.transform.SetLocalPosition({ 3.f, 6.f, 20.f });
    third.transform.LocalScale({ 1.f, 1.f, 1.f });
    MeshRenderer& meshRenderer3 = *third.AddComponent<MeshRenderer>();
    meshRenderer3.SetGeometry(SHAPES.CUBE);
    SphereCollider& col2 = *third.AddComponent<SphereCollider>();
    PhysicComponent& physComp2 = *third.AddComponent<PhysicComponent>();
    physComp2.SetMass(200.f);

    WindowParam windowParam;
    GameManager::Run(windowParam);

    GameManager::Destroy();

END_SUBTEST( StaticCollision )


BEGIN_SUBTEST( Physic, Collision )

    GameManager::Create();
    Scene& scene = Scene::Create();

    Texture bricks( "res/Textures/bricks.jpg" );
    
    GameObject& cameraObject = GameObject::Create( scene );
    cameraObject.transform.LocalTranslate( { 0, 10, -10 } );
    cameraObject.transform.LocalRotate( { 0.35f, 0, 0 } );
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.aspectRatio = 600.0f / 400.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };

    GameObject& ground = GameObject::Create(scene);
    ground.transform.SetLocalPosition({ 6.f, -200.f, 20.f });
    ground.transform.LocalScale({ 400.f,400.f,400.f });
    MeshRenderer& meshRenderer = *ground.AddComponent<MeshRenderer>();
    meshRenderer.SetGeometry(SHAPES.CUBE);
    SphereCollider& col1 = *ground.AddComponent<SphereCollider>();


    GameObject& second = GameObject::Create(scene);
    second.transform.SetLocalPosition({ 9.f, 6.f, 20.f });
    second.transform.LocalScale({ 1.f, 1.f, 1.f });
    MeshRenderer& meshRenderer2 = *second.AddComponent<MeshRenderer>();
    meshRenderer2.SetGeometry(SHAPES.CUBE);
    SphereCollider& col = *second.AddComponent<SphereCollider>();
    PhysicComponent& physComp = *second.AddComponent<PhysicComponent>();
    physComp.SetGravityScale(1.f);
    Force newForce;
    Vector3f32 dir{ -0.5f,1.f,0.f };
    newForce.direction = dir.Normalize();
    newForce.norm = 500.f;
    physComp.AddForce(newForce);


    GameObject& third = GameObject::Create(scene);
    third.transform.SetLocalPosition({ 3.f, 6.f, 20.f });
    third.transform.LocalScale({ 1.f, 1.f, 1.f });
    third.transform.LocalRotate({XM_PIDIV2, 0, 0});
    MeshRenderer& meshRenderer3 = *third.AddComponent<MeshRenderer>();
    meshRenderer3.SetGeometry(SHAPES.CUBE);
    SphereCollider& col2 = *third.AddComponent<SphereCollider>();
    PhysicComponent& physComp2 = *third.AddComponent<PhysicComponent>();
    physComp2.SetGravityScale(1.f);
    physComp2.SetMass(200.f);
    Force newForce2;
    Vector3f32 dir2{ 0.5f,1.f,0.f };
    newForce2.direction = dir2.Normalize();
    newForce2.norm = 4500.f;
    physComp2.AddForce(newForce2);

    GameObject& fourth = GameObject::Create(scene);
    fourth.transform.SetLocalPosition({ 7.f, 3.f, 20.f });
    MeshRenderer& meshRenderer4 = *fourth.AddComponent<MeshRenderer>();
    meshRenderer4.SetGeometry(SHAPES.CUBE);
    SphereCollider& col4 = *fourth.AddComponent<SphereCollider>();
    PhysicComponent& physComp4 = *fourth.AddComponent<PhysicComponent>();
    physComp4.SetGravityScale(0.f);

    GameObject& fifth = GameObject::Create(scene);
    fifth.transform.SetLocalPosition({ 9.f, 3.f, 20.f });
    MeshRenderer& meshRenderer5 = *fifth.AddComponent<MeshRenderer>();
    meshRenderer5.SetGeometry(SHAPES.CUBE);
    SphereCollider& col5 = *fifth.AddComponent<SphereCollider>();
    PhysicComponent& physComp5 = *fifth.AddComponent<PhysicComponent>();
    Force newForce5;
    Vector3f32 dir5{ -0.3f,1.f,0.f };
    newForce5.direction = dir5.Normalize();
    newForce5.norm = 1000.f;
    physComp5.AddForce(newForce5);

    WindowParam windowParam;
    GameManager::Run(windowParam);

    GameManager::Destroy();

END_SUBTEST( Collision )

DECLARE_SCRIPT(PhysicScript, ScriptFlag::Update)

void Update()
{
    timer += gce::GameManager::DeltaTime();
    if (timer > delay && balls < 5)
    {
        timer = 0.f;
        balls++;

        gce::GameObject& second = gce::GameObject::Create(*scene);
        second.transform.SetLocalPosition({ 6, 10, 20 });
        second.transform.LocalScale({ 1, 1, 1 });
        gce::MeshRenderer& meshRenderer2 = *second.AddComponent<gce::MeshRenderer>();
        meshRenderer2.SetGeometry(pCube);
        meshRenderer2.SetAlbedoTexture(defaultTexture);
        gce::SphereCollider& col = *second.AddComponent<gce::SphereCollider>();
        gce::PhysicComponent& physComp = *second.AddComponent<gce::PhysicComponent>();
    }
}

int balls = 0;
float delay = 0.5f;
float timer = 0.f;
gce::Scene* scene;
gce::Geometry* pCube;
gce::D12PipelineObject const* pTexturePso;
gce::Texture* defaultTexture;

END_SCRIPT

DECLARE_SCRIPT(CameraScript, ScriptFlag::FixedUpdate)

void FixedUpdate()
{
    gce::PhysicComponent* p = m_pOwner->GetComponent<gce::PhysicComponent>();
    gce::Force f;
    f.norm = 30000.f;

    if (gce::GetKey(gce::Keyboard::Z))
    {
        f.direction = { 0,0,1 };
        p->AddForce(f);
    }
    if (gce::GetKey(gce::Keyboard::S))
    {
        f.direction = { 0,0,-1 };
        p->AddForce(f);
    }
    if (gce::GetKey(gce::Keyboard::Q))
    {
        f.direction = { -1,0,0 };
        p->AddForce(f);
    }
    if (gce::GetKey(gce::Keyboard::D))
    {
        f.direction = { 1,0,0 };
        p->AddForce(f);
    }
    if (gce::GetKey(gce::Keyboard::A))
    {
        f.direction = { 0,-1,0 };
        p->AddForce(f);
    }
    if (gce::GetKey(gce::Keyboard::E))
    {
        f.direction = { 0,1,0 };
        p->AddForce(f);
    }
    p->SetVelocity({ 0,0,0 });
}

END_SCRIPT

BEGIN_SUBTEST( Physic, Performance )

    GameManager::Create();
    Scene& scene = Scene::Create();

    Texture t1( "res/Textures/bricks.jpg" ); 
    Texture t2( "res/Textures/test1.png" );
    Texture t3( "res/Textures/aaa.png" );
    Texture t4( "res/Textures/stone2_normal.jpg" );

    GameObject& cameraObject = GameObject::Create( scene );
    cameraObject.transform.LocalTranslate( { 0, 10, -10 } );
    cameraObject.transform.LocalRotate( { 0.35f, 0, 0 } );
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.aspectRatio = 600.0f / 400.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };
    cameraObject.AddComponent<PhysicComponent>()->SetGravityScale(0.f);
    cameraObject.AddScript<CameraScript>();
    GameObject& ground = GameObject::Create(scene);
    PhysicScript* script = ground.AddScript<PhysicScript>();
    script->pCube = SHAPES.CUBE;
    script->scene = &scene;
    Texture defaultTexture("res/Textures/bricks.jpg");
    script->defaultTexture = &defaultTexture;
    ground.transform.SetLocalPosition({ 6, -200, 20 });
    ground.transform.LocalScale({ 400,400,400 });
    MeshRenderer& meshRenderer = *ground.AddComponent<MeshRenderer>();
    meshRenderer.SetGeometry(SHAPES.SPHERE);
    SphereCollider& col1 = *ground.AddComponent<SphereCollider>();

    WindowParam windowParam;
    GameManager::Run(windowParam);

    GameManager::Destroy();


END_SUBTEST( Performance )


#endif