#ifndef TEST_GAMETRANSFORM_HPP_INCLUDED
#define TEST_GAMETRANSFORM_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;

BEGIN_TEST( GameTransform )
{
    WindowParam param;
    param.screenDisposition = VERTICAL_2_PLAYERS;
    param.isSplitScreen = true;

    GameManager::Create();
    Scene& scene = Scene::Create();
    
    Texture textures[] {
        Texture( "res/Textures/stone_ambient.jpg" ),
        Texture( "res/Textures/stone_roughness.jpg" ),
        Texture( "res/Textures/stone_color.jpg" ),
    };

    // Camera 1
    {
        GameObject& object = GameObject::Create( scene );
        object.transform.LocalTranslate( { 0, 0, -10 } );

        MeshRenderer& meshRenderer = *object.AddComponent<MeshRenderer>();
        meshRenderer.SetGeometry(SHAPES.CUBE);

        Camera& camera = *object.AddComponent<Camera>();
        camera.SetMainCamera();
        camera.SetType( PERSPECTIVE );
        camera.screenId = 1;
        camera.perspective.fov = PI/4.0f;
        camera.perspective.nearPlane = 0.001f;
        camera.perspective.farPlane = 500.0f;
        camera.perspective.up = { 0.0f, 1.0f, 0.0f };
    }

    GameObject& cameraObject = GameObject::Create( scene );
    cameraObject.AddScript<CameraScript>();
    cameraObject.transform.LocalTranslate( { 0, 0, -10 } );
    MeshRenderer& cameraMeshRenderer = *cameraObject.AddComponent<MeshRenderer>();
    cameraMeshRenderer.SetGeometry(SHAPES.CUBE);
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = PI/4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };
    cameraObject.AddComponent<BoxCollider>();
    cameraObject.AddComponent<PhysicComponent>()->SetGravityScale( 0.0f );
    cameraObject.AddComponent<SkyBoxComponent>()->Create("res/Textures/GrassCube.dds",{100.0f,100.0f,100.0f});
    cameraObject.GetComponent<SkyBoxComponent>()->LockSkyBoxOnCamera(1);

    TransformScript* transformScript = GameObject::Create(scene).AddScript<TransformScript>();
    transformScript->textures[0] = &textures[0];
    transformScript->textures[1] = &textures[1];
    transformScript->textures[2] = &textures[2];

    for ( uint8 i = 0; i < 2; i++ )
    {
        for ( uint8 j = 0; j < 5; j++ )
        {
            GameObject& object = GameObject::Create( scene );
            object.transform.SetLocalPosition( { 2.0f * j - 4.0f, 1.0f - 2.0f * i, 0 } );
            object.AddComponent<BoxCollider>();
            MeshRenderer& meshRenderer = *object.AddComponent<MeshRenderer>();
            meshRenderer.SetGeometry(SHAPES.CUBE);
            meshRenderer.SetAlbedoTexture(transformScript->textures[2]);
            transformScript->gameObjects[i * 5 + j] = &object;
        }
    }

    GameManager::Run( param );

    GameManager::Destroy();

}
END_TEST_MAIN_FUNCTION( GameTransform )

static void HandleMovements( GameObject* const pSelected, bool const world )
{
    if ( pSelected == nullptr) return;

    float32 const deltaTime = GameManager::DeltaTime();

    // Position
    Vector3f32 translation;
    translation.x = static_cast<float32>( GetKey( Keyboard::D ) - GetKey( Keyboard::Q ) );
    translation.y = static_cast<float32>( GetKey( Keyboard::SPACE ) - GetKey( Keyboard::LCONTROL ) );
    translation.z = static_cast<float32>( GetKey( Keyboard::Z ) - GetKey( Keyboard::S ) );
    translation *= deltaTime;

    // Scale
    float32 const allScale = static_cast<float32>( GetKey( Keyboard::NUMPAD_ADD ) - GetKey( Keyboard::NUMPAD_SUBTRACT ) );

    Vector3f32 scale;
    scale.x = static_cast<float32>( GetKey( Keyboard::INSERT ) - GetKey( Keyboard::DELETE_ ) ) + allScale;
    scale.y = static_cast<float32>( GetKey( Keyboard::HOME ) - GetKey( Keyboard::END ) ) + allScale;
    scale.z = static_cast<float32>( GetKey( Keyboard::PAGE_UP ) - GetKey( Keyboard::PAGE_DOWN ) ) + allScale;
    scale *= deltaTime;

    Vector3f32 one;
    one.SetOne();
    scale += one;

    // Rotation
    Vector3f32 rotation;
    rotation.x = static_cast<float32>( GetKey( Keyboard::NUMPAD8 ) - GetKey( Keyboard::NUMPAD5 ) );
    rotation.y = static_cast<float32>( GetKey( Keyboard::NUMPAD4 ) - GetKey( Keyboard::NUMPAD6 ) );
    rotation.z = static_cast<float32>( GetKey( Keyboard::NUMPAD7 ) - GetKey( Keyboard::NUMPAD9 ) );
    rotation *= 3.14f / 180.0f * deltaTime * 50.0f;

    if ( world )
    {
        pSelected->transform.WorldTranslate( translation );
        pSelected->transform.WorldScale( scale );
        pSelected->transform.WorldRotate( { rotation.x, rotation.y, rotation.z } );
        return;
    }

    pSelected->transform.LocalTranslate( translation );
    pSelected->transform.LocalScale( scale );
    pSelected->transform.LocalRotate( { rotation.x, rotation.y, rotation.z } );
}

static void HandleSelection( GameObject* pSelected[2], GameObject* gameObjects[10], Texture textures[3] )
{
    uint32 index;
    if      ( GetKeyDown( Keyboard::_1 ) ) index = 0;
    else if ( GetKeyDown( Keyboard::_2 ) ) index = 1;
    else if ( GetKeyDown( Keyboard::_3 ) ) index = 2;
    else if ( GetKeyDown( Keyboard::_4 ) ) index = 3;
    else if ( GetKeyDown( Keyboard::_5 ) ) index = 4;
    else if ( GetKeyDown( Keyboard::_6 ) ) index = 5;
    else if ( GetKeyDown( Keyboard::_7 ) ) index = 6;
    else if ( GetKeyDown( Keyboard::_8 ) ) index = 7;
    else if ( GetKeyDown( Keyboard::_9 ) ) index = 8;
    else if ( GetKeyDown( Keyboard::_0 ) ) index = 9;
    else return;

    if ( GetKey( Keyboard::LSHIFT ) )
    {
        if ( pSelected[1] ) pSelected[1]->GetComponent<MeshRenderer>()->SetAlbedoTexture(&textures[2]);
        pSelected[1] = gameObjects[index];
        pSelected[1]->GetComponent<MeshRenderer>()->SetAlbedoTexture(&textures[1]);
    }
    else
    {
        if ( pSelected[0] ) pSelected[0]->GetComponent<MeshRenderer>()->SetAlbedoTexture(&textures[2]);
        pSelected[0] = gameObjects[index];
        pSelected[0]->GetComponent<MeshRenderer>()->SetAlbedoTexture(&textures[0]);
    }
}

static void HandleParents( GameObject* pSelected[2] )
{
    if ( pSelected[0] == nullptr || pSelected[1] == nullptr ) return;
    if ( GetKeyDown( Keyboard::² ) == false ) return;

    if ( pSelected[0]->HasParent() ) pSelected[0]->RemoveParent();
    else pSelected[0]->SetParent( *pSelected[1] );
}

static void HandleUsings( GameObject* const pSelected )
{
    if ( pSelected == nullptr ) return;

    if ( GetKeyDown( Keyboard::R ) ) pSelected->transform.ToggleUsing( GameTransform::Using::ROTATION );
    if ( GetKeyDown( Keyboard::T ) ) pSelected->transform.ToggleUsing( GameTransform::Using::POSITION );
    if ( GetKeyDown( Keyboard::Y ) ) pSelected->transform.ToggleUsing( GameTransform::Using::SCALE );
}




static void HandleCameraMovements( GameObject* const pCamera )
{
    if ( pCamera == nullptr ) return;

    float32 const deltaTime = GameManager::DeltaTime();

    // Position
    Vector3f32 translation {};
    translation += pCamera->transform.GetLocalRight() * static_cast<float32>( GetKey( Keyboard::D ) - GetKey( Keyboard::Q ) );
    translation += pCamera->transform.GetLocalUp() * static_cast<float32>( GetKey( Keyboard::SPACE ) - GetKey( Keyboard::LCONTROL ) );
    translation += pCamera->transform.GetLocalForward() * static_cast<float32>( GetKey( Keyboard::Z ) - GetKey( Keyboard::S ) );
    translation *= deltaTime;

    // Scale
    float32 const allScale = static_cast<float32>( GetKey( Keyboard::NUMPAD_ADD ) - GetKey( Keyboard::NUMPAD_SUBTRACT ) );

    Vector3f32 scale;
    scale.x = static_cast<float32>( GetKey( Keyboard::INSERT ) - GetKey( Keyboard::DELETE_ ) ) + allScale;
    scale.y = static_cast<float32>( GetKey( Keyboard::HOME ) - GetKey( Keyboard::END ) ) + allScale;
    scale.z = static_cast<float32>( GetKey( Keyboard::PAGE_UP ) - GetKey( Keyboard::PAGE_DOWN ) ) + allScale;
    scale *= deltaTime;

    Vector3f32 one;
    one.SetOne();
    scale += one;

    Vector2i32 const middlePosition( GameManager::GetWindow()->GetWidth() / 2, GameManager::GetWindow()->GetHeight() / 2 );

    Vector2i32 mouseDirection = GetMousePosition();
    mouseDirection.x -= middlePosition.x;
    mouseDirection.y -= middlePosition.y;

    SetMousePosition( middlePosition );

    // Rotation
    Vector3f32 localRotation {
        static_cast<float32>( mouseDirection.y ),
        0.0f,
        static_cast<float32>( GetKey( Keyboard::NUMPAD7 ) - GetKey( Keyboard::NUMPAD9 ) )
    };
    localRotation *= 3.14f / 180.0f * deltaTime * 5.0f;

    Vector3f32 worldRotation {
        0.0f,
        static_cast<float32>( mouseDirection.x ),
        0.0f
    };
    worldRotation *= 3.14f / 180.0f * deltaTime * 5.0f;

    pCamera->transform.LocalTranslate( translation );
    pCamera->transform.LocalScale( scale );
    pCamera->transform.LocalRotate( localRotation );
    pCamera->transform.WorldRotate( worldRotation );
}

DECLARE_SCRIPT(TransformScript, ScriptFlag::Update)

bool world = false;
GameObject* pSelected[2]{ nullptr, nullptr };
GameObject* gameObjects[10];
Texture* textures[3];

void Update() override
{
    HandleMovements(pSelected[0], world);
    HandleSelection(pSelected, gameObjects, *textures);
    HandleParents(pSelected);
    HandleUsings(pSelected[0]);
    if ( GetKeyDown( Keyboard::G ) ) world ^= true;
    if ( GetKeyDown( Keyboard::BACKSPACE ) )
    {
        pSelected[0]->Destroy();
        pSelected[0] = nullptr;
    }
}

END_SCRIPT

DECLARE_SCRIPT(CameraScript, ScriptFlag::Update)

void Update() override
{
    if (GetKeyDown(Keyboard::L))
    {
        if (IsMouseCursorLocked()) UnlockMouseCursor();
        else LockMouseCursor();
    }
    if (GetKeyDown(Keyboard::H))
    {
        if (IsMouseCursorVisible()) HideMouseCursor();
        else ShowMouseCursor();
    }
    HandleCameraMovements(m_pOwner);
}

END_SCRIPT

END_TEST( GameTransform )



#endif