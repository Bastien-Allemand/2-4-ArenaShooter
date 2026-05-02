#ifndef TEST_EVENT_HPP_INCLUDED
#define TEST_EVENT_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Event )

    GameManager::Create();
    Scene& scene = Scene::Create();

    Texture texture( "res/Textures/bricks.jpg" );

    GameObject& cameraObject = GameObject::Create( scene );
    cameraObject.transform.LocalTranslate( { 0, 0, -10 } );
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };

    GameObject& object = GameObject::Create( scene );
    object.AddScript<EventScript>()->event.AddListener([](GameObject*, cstr const message) { Console::Log(message, Console::GREEN); });

    MeshRenderer& meshRenderer = *object.AddComponent<MeshRenderer>();
    meshRenderer.SetGeometry(SHAPES.CUBE);
    meshRenderer.SetAlbedoTexture(&texture);

    WindowParam windowParam;
    GameManager::Run( windowParam );

    GameManager::Destroy();

END_TEST_MAIN_FUNCTION( Event )

DECLARE_SCRIPT(EventScript, ScriptFlag::Update)
Event<GameObject*, cstr> event;
Event<GameObject*, cstr>::ListenerBase* pListener = nullptr;
void Update()
{
    if ( GetKeyDown( Keyboard::RETURN ) ) event.Invoke( m_pOwner, "Pressed Return!" );
    if ( GetKeyDown( Keyboard::SPACEBAR ) ) pListener = event.AddListener( []( GameObject* const pObject2, cstr ){ pObject2->transform.LocalScale( { 1.5f, 1.5f, 1.5f } ); } );
    if ( GetKeyDown( Keyboard::BACKSPACE ) ) event.ClearListeners();
    if ( GetKeyDown( Keyboard::DELETE_ ) ) event.RemoveListener( pListener );
}

END_SCRIPT
END_TEST( Event )


#endif