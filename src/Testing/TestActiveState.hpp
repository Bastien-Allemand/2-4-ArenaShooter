#ifndef TEST_ACTIVE_STATE_HPP_INCLUDED
#define TEST_ACTIVE_STATE_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( ActiveState )

	GameManager::Create();
	Scene& scene = Scene::Create();

	GameObject& cameraObject = GameObject::Create( scene );
	cameraObject.transform.LocalTranslate( { 0, 0, -10 } );
	Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
	camera.SetType( PERSPECTIVE );
	camera.perspective.fov = XM_PIDIV4;
	camera.perspective.nearPlane = 0.001f;
	camera.perspective.farPlane = 500.0f;
	camera.perspective.aspectRatio = 600.0f / 400.0f;
	camera.perspective.up = { 0.0f, 1.0f, 0.0f };

	GameObject& testObject = GameObject::Create( scene );
	MeshRenderer& meshRenderer = *testObject.AddComponent<MeshRenderer>();
	meshRenderer.SetGeometry(SHAPES.CUBE);

	GameObject& gameManager = GameObject::Create( scene );
	gameManager.AddScript<ActiveStateTestManager>()->testObject = &testObject;

	WindowParam windowParam;
	GameManager::Run(windowParam);

	GameManager::Destroy();

END_TEST_MAIN_FUNCTION()

DECLARE_SCRIPT(ActiveStateTestManager, ScriptFlag::Update)

uint8 i = 1;
gce::GameObject* testObject;

void Update()
{

	if (GetKeyDown(Keyboard::SPACE) == false) return;

	MeshRenderer& meshRenderer = *testObject->GetComponent<MeshRenderer>();

	switch (i)
	{
		// Object Active Component Active
	case 0:
		testObject->SetActive(true);
		meshRenderer.SetActive(true);
		// assert(testObject.IsActive() == true);
		// assert(meshRenderer.IsActive() == true);
		break;

		// Object Active Component Inactive
	case 1:
		testObject->SetActive(true);
		meshRenderer.SetActive(false);
		// assert(testObject.IsActive() == true);
		// assert(meshRenderer.IsActive() == false);
		break;

		// Object Inactive Component Active
	case 2:
		testObject->SetActive(false);
		meshRenderer.SetActive(true);
		// assert(testObject.IsActive() == false);
		// assert(meshRenderer.IsActive() == false);
		break;

		// Object Inactive Component Inactive
	case 3:
		testObject->SetActive(false);
		meshRenderer.SetActive(false);
		// assert(testObject.IsActive() == false);
		// assert(meshRenderer.IsActive() == false);
		break;

	default: break;
	}

	i++;
	if (i > 3) i = 0;
}

END_SCRIPT



END_TEST( ActiveState )

#endif