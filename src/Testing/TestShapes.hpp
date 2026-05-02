#ifndef TEST_SHAPES_HPP_INCLUDED
#define TEST_SHAPES_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Shapes )

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

	// Cube
	{
		GameObject& cube = GameObject::Create( scene );
		cube.AddScript<ShapeRotateScript>();
		MeshRenderer& meshRenderer = *cube.AddComponent<MeshRenderer>();
		meshRenderer.SetGeometry(SHAPES.CUBE);
		cube.transform.SetLocalPosition( { -4.5f, 0, 0 } );
	}

	// Sphere
	{
		GameObject& sphere = GameObject::Create( scene );
		sphere.AddScript<ShapeRotateScript>();
		MeshRenderer& meshRenderer = *sphere.AddComponent<MeshRenderer>();
		meshRenderer.SetGeometry(SHAPES.SPHERE);
		sphere.transform.SetLocalPosition( { -3.f, 0, 0 } );
	}

	// Capsule
	{
		GameObject& capsule = GameObject::Create( scene );
		capsule.AddScript<ShapeRotateScript>();
		MeshRenderer& meshRenderer = *capsule.AddComponent<MeshRenderer>();
		meshRenderer.SetGeometry(SHAPES.CAPSULE);
		capsule.transform.SetLocalPosition( { -1.5f, 0, 0 } );
	}

    // Cylinder
	{
		GameObject& cylinder = GameObject::Create( scene );
		cylinder.AddScript<ShapeRotateScript>();
		MeshRenderer& meshRenderer = *cylinder.AddComponent<MeshRenderer>();
		meshRenderer.SetGeometry(SHAPES.CYLINDER);
		cylinder.transform.SetLocalPosition( { 0, 0, 0 } );
	}

	// Half Sphere
	{
		GameObject& halfSphere = GameObject::Create( scene );
		halfSphere.AddScript<ShapeRotateScript>();
		MeshRenderer& meshRenderer = *halfSphere.AddComponent<MeshRenderer>();
		meshRenderer.SetGeometry(SHAPES.HALF_SPHERE);
		halfSphere.transform.SetLocalPosition( { 1.5f, 0, 0 } );
	}

    // Donut
	{
		GameObject& donut = GameObject::Create( scene );
		donut.AddScript<ShapeRotateScript>();
		MeshRenderer& meshRenderer = *donut.AddComponent<MeshRenderer>();
		meshRenderer.SetGeometry(SHAPES.DONUT);
		donut.transform.SetLocalPosition( { 3.f, 0, 0 } );
	}

	// Plane
	{
		GameObject& plane = GameObject::Create( scene );
		plane.AddScript<ShapeRotateScript>();
		MeshRenderer& meshRenderer = *plane.AddComponent<MeshRenderer>();
		meshRenderer.SetGeometry(SHAPES.PLANE);
		plane.transform.SetLocalPosition( { 4.5f, 0, 0 } );
	}

	WindowParam windowParam;
	GameManager::Run(windowParam);

	GameManager::Destroy();

END_TEST_MAIN_FUNCTION( Shapes )
DECLARE_SCRIPT(ShapeRotateScript, ScriptFlag::Update)

void Update()
{
	float32 const deltaTime = GameManager::DeltaTime();
	m_pOwner->transform.LocalRotate({
		static_cast<float32>(GetKey(Keyboard::D) - GetKey(Keyboard::Q)) * 10.0f * deltaTime,
		static_cast<float32>(GetKey(Keyboard::Z) - GetKey(Keyboard::S)) * 10.0f * deltaTime,
		0.0f
		});
}

END_SCRIPT


END_TEST( Shapes )


#endif