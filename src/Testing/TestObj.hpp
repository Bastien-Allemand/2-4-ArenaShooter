#ifndef TEST_OBJ_HPP_INCLUDED
#define TEST_OBJ_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Obj )

    GameManager::Create();

    Scene& scene = Scene::Create();

    D12PipelineObject pso{ SHADERS.VERTEX, SHADERS.PIXEL, SHADERS.HULL, SHADERS.DOMAIN_, SHADERS.ROOT_SIGNATURE };

    Geometry* pCustomGeo1 = GeometryFactory::LoadGeometry(RES_PATH"res/Test/Obj/bugatti.obj");
    Geometry* pCustomGeo2 = GeometryFactory::LoadGeometry(RES_PATH"res/Test/Obj/barrel_large.obj");
    Geometry* pCustomGeo3 = GeometryFactory::LoadGeometry(RES_PATH"res/Test/Obj/Penguin.obj");

    GameObject& object1 = GameObject::Create(scene);
    object1.AddScript<ObjMovement>();
    object1.transform.SetWorldPosition({ 2, 0, 10 });
    MeshRenderer* meshRenderer1 = object1.AddComponent<MeshRenderer>();
    meshRenderer1->SetGeometry(pCustomGeo1);

    GameObject& object2 = GameObject::Create(scene);
    object2.AddScript<ObjMovement>();
    object2.transform.SetWorldPosition({ 0, 0, 0 });
    MeshRenderer* meshRenderer2 = object2.AddComponent<MeshRenderer>();
    meshRenderer2->SetGeometry(pCustomGeo2);

    GameObject& object3 = GameObject::Create(scene);
    object3.AddScript<ObjMovement>();
    object3.transform.SetWorldPosition({ -2, 0, 0 });
    MeshRenderer* meshRenderer3 = object3.AddComponent<MeshRenderer>();
    meshRenderer3->SetGeometry(pCustomGeo3);

    GameObject& cam = GameObject::Create(scene);
    cam.transform.SetWorldPosition({ 0.0f,0.0f,-10.0f });
    Camera* camera = cam.AddComponent<Camera>();
    camera->SetMainCamera();
    camera->perspective.fov = XM_PIDIV4;
    camera->perspective.farPlane = 500.0f;
    camera->perspective.nearPlane = 0.001f;
    camera->perspective.up = { 0.0f, 1.0f, 0.0f };
    camera->SetType(PERSPECTIVE);

    WindowParam windowParam;

    GameManager::Run(windowParam);

    delete pCustomGeo1;
    delete pCustomGeo2;
    delete pCustomGeo3;

    GameManager::Destroy();

END_TEST_MAIN_FUNCTION( Obj )

DECLARE_SCRIPT( ObjMovement, ScriptFlag::Update )

void Update() override
{
    if (m_pOwner == nullptr) return;

    float32 const deltaTime = GameManager::DeltaTime();

    // Position
    Vector3f32 translation;
    translation.x = static_cast<float32>(GetKey(Keyboard::D) - GetKey(Keyboard::Q));
    translation.y = static_cast<float32>(GetKey(Keyboard::SPACE) - GetKey(Keyboard::LCONTROL));
    translation.z = static_cast<float32>(GetKey(Keyboard::Z) - GetKey(Keyboard::S));
    translation *= deltaTime;

    // Scale
    float32 const allScale = static_cast<float32>(GetKey(Keyboard::NUMPAD_ADD) - GetKey(Keyboard::NUMPAD_SUBTRACT));

    Vector3f32 scale;
    scale.x = static_cast<float32>(GetKey(Keyboard::INSERT) - GetKey(Keyboard::DELETE_)) + allScale;
    scale.y = static_cast<float32>(GetKey(Keyboard::HOME) - GetKey(Keyboard::END)) + allScale;
    scale.z = static_cast<float32>(GetKey(Keyboard::PAGE_UP) - GetKey(Keyboard::PAGE_DOWN)) + allScale;
    scale *= deltaTime;

    Vector3f32 one;
    one.SetOne();
    scale += one;

    // Rotation
    Vector3f32 rotation;
    rotation.x = static_cast<float32>(GetKey(Keyboard::NUMPAD8) - GetKey(Keyboard::NUMPAD5));
    rotation.y = static_cast<float32>(GetKey(Keyboard::NUMPAD4) - GetKey(Keyboard::NUMPAD6));
    rotation.z = static_cast<float32>(GetKey(Keyboard::NUMPAD7) - GetKey(Keyboard::NUMPAD9));
    rotation *= 3.14f / 180.0f * deltaTime * 50.0f;

    m_pOwner->transform.LocalTranslate(translation);
    m_pOwner->transform.LocalScale(scale);
    m_pOwner->transform.LocalRotate({ rotation.x, rotation.y, rotation.z });
}

END_SCRIPT

END_TEST( Obj )


#endif