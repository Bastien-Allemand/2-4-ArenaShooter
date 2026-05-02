#ifndef TESTING_TEST_UI_IMAGE_H_INCLUDED
#define TESTING_TEST_UI_IMAGE_H_INCLUDED

#include "define.h"

#include "Console.h"
#include <Engine.h>
#include <Render.h>

#include "Components/UiImage.h"

using namespace gce;

BEGIN_TEST( UiImage )

    GameManager::Create();
    Scene& scene = Scene::Create();

    Console::Log("WindowParam Create", Console::BLUE);
    WindowParam windowParam;
    windowParam.title = L"Test Light";
    windowParam.width = 1080;
    windowParam.height = 720;
    windowParam.isFullScreen = true;

    GameObject& crosshair = GameObject::Create(scene);
    UiImage& uiImage = *crosshair.AddComponent<UiImage>();
    Vector2f32 center = { windowParam.width / 2.f, windowParam.height / 2.f };
    Vector2f32 size = { 32, 32 };
    Vector2f32 posUi = center - size * 0.5f;
    uiImage.InitializeImage(posUi,size, 1.f);

    uiImage.btmBrush = new BitMapBrush("res/Textures/crosshair.png");
    uiImage.btmBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 },{ 1.f / 16.f, 1.f / 16.f, 1.f / 16.f }, 0);
	uiImage.SetActive(true);



    HideMouseCursor();
    GameObject& Cube = GameObject::Create(scene);
    MeshRenderer* meshRenderer = Cube.AddComponent<MeshRenderer>();
    meshRenderer->SetGeometry(SHAPES.CUBE);
    Cube.transform.SetWorldPosition({0, 0, 10});

    GameObject& cameraObject = GameObject::Create(scene);
    Camera* camera = cameraObject.AddComponent<Camera>();
    cameraObject.AddScript<CameraScript>();
    camera->SetType(PERSPECTIVE);
    camera->perspective.fov = XM_PIDIV4;
    camera->perspective.nearPlane = 0.001f;
    camera->perspective.farPlane = 500.0f;
    camera->perspective.up = { 0.0f, 1.0f, 0.0f };

    GameManager::Run( windowParam );
    
    GameManager::Destroy();

END_TEST_MAIN_FUNCTION( ImageUiComponent )

DECLARE_SCRIPT( CameraScript, ScriptFlag::Update )

    void Update() override
    {
        if (m_pOwner == nullptr) return;

        float32 const deltaTime = GameManager::DeltaTime();

        // Position
        Vector3f32 translation{};
        translation += m_pOwner->transform.GetLocalRight() * static_cast<float32>(GetKey(Keyboard::D) - GetKey(Keyboard::Q));
        translation += m_pOwner->transform.GetLocalUp() * static_cast<float32>(GetKey(Keyboard::SPACE) - GetKey(Keyboard::LCONTROL));
        translation += m_pOwner->transform.GetLocalForward() * static_cast<float32>(GetKey(Keyboard::Z) - GetKey(Keyboard::S));
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

        Vector2i32 const middlePosition(GameManager::GetWindow()->GetWidth() / 2, GameManager::GetWindow()->GetHeight() / 2);

        Vector2i32 mouseDirection = GetMousePosition();
        mouseDirection.x -= middlePosition.x;
        mouseDirection.y -= middlePosition.y;

        SetMousePosition(middlePosition);

        // Rotation
        Vector3f32 localRotation{
            static_cast<float32>(mouseDirection.y),
            0.0f,
            static_cast<float32>(GetKey(Keyboard::NUMPAD7) - GetKey(Keyboard::NUMPAD9))
        };
        localRotation *= 3.14f / 180.0f * deltaTime * 5.0f;

        Vector3f32 worldRotation{
            0.0f,
            static_cast<float32>(mouseDirection.x),
            0.0f
        };
        worldRotation *= 3.14f / 180.0f * deltaTime * 5.0f;

        m_pOwner->transform.LocalTranslate(translation);
        m_pOwner->transform.LocalScale(scale);
        m_pOwner->transform.LocalRotate(localRotation);
        m_pOwner->transform.WorldRotate(worldRotation);
	}

END_SCRIPT

END_TEST( ImageUiComponent )

#endif