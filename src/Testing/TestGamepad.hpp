#ifndef TEST_GAMEPAD_HPP_INCLUDED
#define TEST_GAMEPAD_HPP_INCLUDED

#include <Maths/Vector3.h>
#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Gamepad )

    GameManager::Create();
    Scene& scene = Scene::Create();

    GameObject& cameraObject = GameObject::Create( scene );
    cameraObject.transform.LocalTranslate( { 0.0f, 0.0f, -40.0f } );
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };

    GameObject* pControllers[4];
    BuildController<0>( scene,  pControllers, { -25.0f,  15.0f, 30.0f } );
    BuildController<1>( scene,  pControllers, {  25.0f,  15.0f, 30.0f } );
    BuildController<2>( scene,  pControllers, { -25.0f, -15.0f, 30.0f } );
    BuildController<3>( scene, pControllers, {  25.0f, -15.0f, 30.0f } );

    GameManagerScript& script = *cameraObject.AddScript<GameManagerScript>();
    memcpy( script.pControllers, pControllers, sizeof( pControllers ) );

    WindowParam windowParam;
    GameManager::Run( windowParam );

    GameManager::Destroy();

END_TEST_MAIN_FUNCTION( Gamepad )


template <uint8 controllerIndex>
static void BuildController( Scene& scene, GameObject* pControllers[4], Vector3f32 const& position )
{
    GameObject& controller = GameObject::Create( scene );
    pControllers[controllerIndex] = &controller;
    
    // ===== Right Buttons ===== //

    {
        GameObject& rightButtons = GameObject::Create( scene );
        rightButtons.SetParent( controller );

        // Y
        {
            GameObject& buttonY = GameObject::Create( scene );
            FaceButtonCallback& script = *buttonY.AddScript<FaceButtonCallback>();
            script.button = Gamepad::Y;
            script.key = Keyboard::Y;
            script.controllerIndex = controllerIndex;
            buttonY.SetParent( rightButtons );
            buttonY.transform.LocalTranslate( { 0.0f, 1.0f, 0.0f } );
            MeshRenderer& meshRenderer = *buttonY.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.SPHERE);
        }

        // X
        {
            GameObject& buttonX = GameObject::Create( scene );
            FaceButtonCallback& script = *buttonX.AddScript<FaceButtonCallback>();
            script.button = Gamepad::X;
            script.key = Keyboard::X;
            script.controllerIndex = controllerIndex;
            buttonX.SetParent( rightButtons );
            buttonX.transform.LocalTranslate( { -1.0f, 0.0f, 0.0f } );
            MeshRenderer& meshRenderer = *buttonX.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.SPHERE);
        }

        // B
        {
            GameObject& buttonB = GameObject::Create( scene );
            FaceButtonCallback& script = *buttonB.AddScript<FaceButtonCallback>();
            script.button = Gamepad::B;
            script.key = Keyboard::B;
            script.controllerIndex = controllerIndex;
            buttonB.SetParent( rightButtons );
            buttonB.transform.LocalTranslate( { 1.0f, 0.0f, 0.0f } );
            MeshRenderer& meshRenderer = *buttonB.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.SPHERE);
        }

        // A
        {
            GameObject& buttonA = GameObject::Create( scene );
            FaceButtonCallback& script = *buttonA.AddScript<FaceButtonCallback>();
            script.button = Gamepad::A;
            script.key = Keyboard::A;
            script.controllerIndex = controllerIndex;
            buttonA.SetParent( rightButtons );
            buttonA.transform.LocalTranslate( { 0.0f, -1.0f, 0.0f } );
            MeshRenderer& meshRenderer = *buttonA.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.SPHERE);
        }

        rightButtons.transform.LocalTranslate( { 12.0f, 0.0f, 0.0f } );
        rightButtons.transform.LocalScale( { 2.0f, 2.0f, 1.0f } );
    }

    // ===== Left Arrows ===== //

    {
        GameObject& leftArrows = GameObject::Create( scene );
        ArrowsCallback& script = *leftArrows.AddScript<ArrowsCallback>();
        script.controllerIndex = controllerIndex;
        leftArrows.SetParent( controller );

        // Vertical
        {
            GameObject& vertical = GameObject::Create( scene );
            vertical.SetParent( leftArrows );
            vertical.transform.LocalScale( { 3.0f, 1.0f, 1.0f } );
            MeshRenderer& meshRenderer = *vertical.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.CUBE);
        }

        // Horizontal
        {
            GameObject& horizontal = GameObject::Create( scene );
            horizontal.SetParent( leftArrows );
            horizontal.transform.LocalScale( { 1.0f, 3.0f, 1.0f } );
            MeshRenderer& meshRenderer = *horizontal.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.CUBE);
        }

        leftArrows.transform.LocalTranslate( { -6.0f, -6.0f, 0.0f } );
        leftArrows.transform.LocalScale( { 1.5f, 1.5f, 1.0f } );
    }

    // ===== Joysticks ===== //

    {
        // Left
        {
            GameObject& leftJoystick = GameObject::Create( scene );
            JoystickCallback& script = *leftJoystick.AddScript<JoystickCallback>();
            script.pGetStickFunction = &GetLeftStick;
            script.button = Gamepad::LEFT_THUMB;
            script.controllerIndex = controllerIndex;
            script.position = { -12.0f, 0.0f, 0.0f };
            leftJoystick.SetParent( controller );
            leftJoystick.transform.LocalTranslate( { -12.0f, 0.0f, 0.0f } );
            leftJoystick.transform.LocalRotate( { XM_PIDIV2, 0.0f, 0.0f } );
            leftJoystick.transform.LocalScale( { 3.0f, 3.0f, 3.0f } );
            MeshRenderer& meshRenderer = *leftJoystick.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.DONUT);
        }

        // Right
        {
            GameObject& rightJoystick = GameObject::Create( scene );
            JoystickCallback& script = *rightJoystick.AddScript<JoystickCallback>();
            script.pGetStickFunction = &GetRightStick;
            script.button = Gamepad::RIGHT_THUMB;
            script.controllerIndex = controllerIndex;
            script.position = { 6.0f, -6.0f, 0.0f };
            rightJoystick.SetParent( controller );
            rightJoystick.transform.LocalTranslate( { 6.0f, -6.0f, 0.0f } );
            rightJoystick.transform.LocalRotate( { XM_PIDIV2, 0.0f, 0.0f } );
            rightJoystick.transform.LocalScale( { 3.0f, 3.0f, 3.0f } );
            MeshRenderer& meshRenderer = *rightJoystick.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.DONUT);
        }
    }

    // ===== Triggers ===== //

    {
        // Left
        {
            GameObject& leftTrigger = GameObject::Create( scene );
            TriggerCallback& script = *leftTrigger.AddScript<TriggerCallback>();
            script.pGetTriggerFunction = &GetLeftTrigger;
            script.key = Keyboard::LSHIFT;
            script.motorSpeeds = { 1.0f, 0.0f };
            script.controllerIndex = controllerIndex;
            script.position = { -12.0f, 8.0f, 0.0f };
            script.scale = { 1.5f, 2.5f, 4.5f };
            leftTrigger.SetParent( controller );
            leftTrigger.transform.LocalTranslate( { -12.0f, 8.0f, 0.0f } );
            leftTrigger.transform.LocalScale( { 1.5f, 2.5f, 4.5f } );
            MeshRenderer& meshRenderer = *leftTrigger.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.CUBE);
        }

        // Right
        {
            GameObject& rightTrigger = GameObject::Create( scene );
            TriggerCallback& script = *rightTrigger.AddScript<TriggerCallback>();
            script.pGetTriggerFunction = &GetRightTrigger;
            script.key = Keyboard::RSHIFT;
            script.motorSpeeds = { 0.0f, 1.0f };
            script.controllerIndex = controllerIndex;
            script.position = { 12.0f, 8.0f, 0.0f };
            script.scale = { 1.5f, 2.5f, 4.5f };
            rightTrigger.SetParent( controller );
            rightTrigger.transform.LocalTranslate( { 12.0f, 8.0f, 0.0f } );
            rightTrigger.transform.LocalScale( { 1.5f, 2.5f, 4.5f } );
            MeshRenderer& meshRenderer = *rightTrigger.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.CUBE);
        }
    }

    // ===== Shoulder Buttons ===== //

    {
        // Left
        {
            GameObject& leftShoulder = GameObject::Create( scene );
            TopButtonCallback& script = *leftShoulder.AddScript<TopButtonCallback>();
            script.button = Gamepad::LEFT_SHOULDER;
            script.key = Keyboard::LCTRL;
            script.controllerIndex = controllerIndex;
            leftShoulder.SetParent( controller );
            leftShoulder.transform.LocalTranslate( { -12.0f, 5.5f, 0.0f } );
            leftShoulder.transform.LocalScale( { 4.0f, 1.0f, 1.5f } );
            MeshRenderer& meshRenderer = *leftShoulder.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.CUBE);
        }

        // Right
        {
            GameObject& rightShoulder = GameObject::Create( scene );
            TopButtonCallback& script = *rightShoulder.AddScript<TopButtonCallback>();
            script.button = Gamepad::RIGHT_SHOULDER;
            script.key = Keyboard::RCTRL;
            script.controllerIndex = controllerIndex;
            rightShoulder.SetParent( controller );
            rightShoulder.transform.LocalTranslate( { 12.0f, 5.5f, 0.0f } );
            rightShoulder.transform.LocalScale( { 4.0f, 1.0f, 1.5f } );
            MeshRenderer& meshRenderer = *rightShoulder.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.CUBE);
        }
    }

    // ===== Middle Buttons ===== //

    {
        // Select
        {
            GameObject& selectButton = GameObject::Create( scene );
            FaceButtonCallback& script = *selectButton.AddScript<FaceButtonCallback>();
            script.button = Gamepad::SELECT;
            script.key = Keyboard::DELETE_;
            script.controllerIndex = controllerIndex;
            selectButton.SetParent( controller );
            selectButton.transform.LocalTranslate( { -3.0f, 0.0f, 0.0f } );
            selectButton.transform.LocalScale( { 1.25f, 1.0f, 1.0f } );
            MeshRenderer& meshRenderer = *selectButton.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.SPHERE);
        }

        // Start
        {
            GameObject& startButton = GameObject::Create( scene );
            FaceButtonCallback& script = *startButton.AddScript<FaceButtonCallback>();
            script.button = Gamepad::START;
            script.key = Keyboard::PAGE_DOWN;
            script.controllerIndex = controllerIndex;
            startButton.SetParent( controller );
            startButton.transform.LocalTranslate( { 3.0f, 0.0f, 0.0f } );
            startButton.transform.LocalScale( { 1.25f, 1.0f, 1.0f } );
            MeshRenderer& meshRenderer = *startButton.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.SPHERE);
        }
    }

    controller.transform.LocalTranslate( position );
    controller.SetActive( false );
}


// ===== Buttons ===== //

DECLARE_SCRIPT( FaceButtonCallback, ScriptFlag::Update )

void Update() override
{
    float32 z = 0.75f;
    if ( IsGamepadConnected() ) z *= static_cast<float32>( GetButtonDown( button, controllerIndex ) - GetButtonUp( button, controllerIndex ) );
    else z *= static_cast<float32>( GetKeyDown( key ) - GetKeyUp( key ) );
    m_pOwner->transform.LocalTranslate( { 0.0f, 0.0f, z } );
}

Gamepad::Button button;
Keyboard::Key key;
uint8 controllerIndex;

END_SCRIPT

DECLARE_SCRIPT( TopButtonCallback, ScriptFlag::Update )

void Update() override
{
    float32 y = 0.25f;
    if ( IsGamepadConnected() ) y *= static_cast<float32>( GetButtonUp( button, controllerIndex ) - GetButtonDown( button, controllerIndex ) );
    else y *= static_cast<float32>( GetKeyUp( key ) - GetKeyDown( key ) );
    m_pOwner->transform.LocalTranslate( { 0.0f, y, 0.0f } );
}

Gamepad::Button button;
Keyboard::Key key;
uint8 controllerIndex;

END_SCRIPT


// ===== Arrows ===== //

DECLARE_SCRIPT( ArrowsCallback, ScriptFlag::Update )

void Update() override
{
    Quaternion rotation;
    if ( IsGamepadConnected() )
        rotation.SetRotationEuler(
            static_cast<float32>( GetButton( Gamepad::UP, controllerIndex ) - GetButton( Gamepad::DOWN, controllerIndex ) ) * 0.3f,
            static_cast<float32>( GetButton( Gamepad::LEFT, controllerIndex ) - GetButton( Gamepad::RIGHT, controllerIndex ) ) * 0.3f,
            0.0f
        );
    else rotation.SetRotationEuler(
            static_cast<float32>( GetKey( Keyboard::UP ) - GetKey( Keyboard::DOWN ) ) * 0.3f,
            static_cast<float32>( GetKey( Keyboard::LEFT ) - GetKey( Keyboard::RIGHT ) ) * 0.3f,
            0.0f
        );
    m_pOwner->transform.SetLocalRotation( rotation );
}

uint8 controllerIndex;

END_SCRIPT


// ===== Joysticks ===== //

DECLARE_SCRIPT( JoystickCallback, ScriptFlag::Update )

void Update() override
{
    Vector3f32 translation {};
    if ( IsGamepadConnected() )
    {
        translation.x = pGetStickFunction( controllerIndex ).x;
        translation.y = pGetStickFunction( controllerIndex ).y;
        if ( translation.x < 0.0f ) translation.x *= -1.0f;
        if ( translation.y < 0.0f ) translation.y *= -1.0f;
        translation.SelfNormalize();
        translation.x *= pGetStickFunction( controllerIndex ).x * 1.75f;
        translation.y *= pGetStickFunction( controllerIndex ).y * 1.75f;
        translation.z = static_cast<float32>( GetButton( button, controllerIndex ) ) * 0.3f;
    }
    else return;
    m_pOwner->transform.SetLocalPosition( position + translation );
}

Vector2f32(*pGetStickFunction)(uint8);
Gamepad::Button button;
uint8 controllerIndex;
Vector3f32 position;

END_SCRIPT


// ===== Triggers ===== //

DECLARE_SCRIPT( TriggerCallback, ScriptFlag::Update )

void Update() override
{
    float32 y = 1.0f;
    if ( IsGamepadConnected() ) y *= pGetTriggerFunction( controllerIndex );
    else y *= static_cast<float32>( GetKey( key ) );
    m_pOwner->transform.SetLocalPosition( { position.x, position.y - scale.y * 0.5f * y, position.z } );
    SetGamepadVibration( motorSpeeds.x * y, motorSpeeds.y * y, controllerIndex );
    y = 1.0f - y;
    m_pOwner->transform.SetLocalScale( { scale.x, scale.y * y, scale.z } );
}

float32(*pGetTriggerFunction)(uint8);
Keyboard::Key key;
Vector2f32 motorSpeeds;
uint8 controllerIndex;
Vector3f32 position;
Vector3f32 scale;

END_SCRIPT


DECLARE_SCRIPT( GameManagerScript, ScriptFlag::Update )

void Update() override
{
    if ( IsGamepadConnected() == false )
    {
        pControllers[0]->SetActive( true );
        return;
    }

    for ( uint8 i = 0; i < 4; i++ )
        pControllers[i]->SetActive( IsGamepadConnected( i ) );
}

GameObject* pControllers[4];

END_SCRIPT


END_TEST( Gamepad )


#endif