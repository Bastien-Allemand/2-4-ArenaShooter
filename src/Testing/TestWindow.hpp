#ifndef TEST_WINDOW_HPP_INCLUDED
#define TEST_WINDOW_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;

BEGIN_TEST( Window )

	GameManager::Create();

    Scene& scene = Scene::Create();

    GameObject& cube = GameObject::Create(scene);
    cube.AddScript<WindowScript1>();
    cube.transform.SetWorldPosition({ -1.0f, 1.0f, 0.0f });
    cube.SetScreenToDraw(0);
    MeshRenderer& meshRenderer = *cube.AddComponent<MeshRenderer>();
    meshRenderer.SetGeometry(SHAPES.CUBE);

	GameObject& cameraObject = GameObject::Create( scene );
	cameraObject.transform.LocalTranslate( { 0.0f, 0.0f, -10.0f } );
	Camera& camera = *cameraObject.AddComponent<Camera>();
	camera.SetType( PERSPECTIVE );
	camera.perspective.fov = XM_PIDIV4;
	camera.perspective.nearPlane = 0.001f;
	camera.perspective.farPlane = 500.0f;
	camera.perspective.up = { 0.0f, 1.0f, 0.0f };


    GameObject& cylinder = GameObject::Create(scene);
    cylinder.AddScript<WindowScript1>();
    cylinder.transform.SetWorldPosition({ 1.0f, 1.0f, 0.0f });
    cylinder.SetScreenToDraw(1);
    MeshRenderer& meshRenderer2 = *cylinder.AddComponent<MeshRenderer>();
    meshRenderer2.SetGeometry(SHAPES.CYLINDER);

    GameObject& sphere = GameObject::Create(scene);
    sphere.AddScript<WindowScript1>();
    sphere.transform.SetWorldPosition({ -1.0f, -1.0f, 0.0f });
    sphere.SetScreenToDraw(2);
    MeshRenderer& meshRenderer3 = *sphere.AddComponent<MeshRenderer>();
    meshRenderer3.SetGeometry(SHAPES.SPHERE);

    GameObject& capsule = GameObject::Create(scene);
    capsule.AddScript<WindowScript1>();
    capsule.transform.SetWorldPosition({ 1.0f, -1.0f, 0.0f });
    capsule.SetScreenToDraw(3);
    MeshRenderer& meshRenderer4 = *capsule.AddComponent<MeshRenderer>();
    meshRenderer4.SetGeometry(SHAPES.CAPSULE);

	WindowParam windowParam;
	windowParam.title = L"Split Screen Test";
	windowParam.width = 1280;
	windowParam.height = 720;
	windowParam.isFullScreen = false;
	windowParam.isSplitScreen = true;
	windowParam.screenDisposition = SplitScreenDisposition::SQUARE_4_PLAYERS;

    GameManager::Run(windowParam);

    GameManager::Destroy();

END_TEST_MAIN_FUNCTION( Window )

DECLARE_SCRIPT(WindowScript1, ScriptFlag::Update);

void Update()
{
    m_pOwner->transform.WorldRotate({ 0.0f, 0.8f * GameManager::DeltaTime(), 0.0f });
}

END_SCRIPT

END_TEST( Window )


#endif