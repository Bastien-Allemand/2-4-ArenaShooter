#ifndef TEST_COLLIDER_2D_HPP_INCLUDED
#define TEST_COLLIDER_2D_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include <Shapes.h>
#include "define.h"

using namespace gce;

BEGIN_TEST( Collider2D )

	GameManager::Create();

	Scene& scene = Scene::Create();

	D12PipelineObject pso(
		SHADERS.VERTEX,
		SHADERS.PIXEL,
		SHADERS.HULL,
		SHADERS.DOMAIN_,
		SHADERS.ROOT_SIGNATURE
	);


	WindowParam windowParam;
	windowParam.width = 720;
	windowParam.height = 720;

	GameObject& cameraObject = GameObject::Create(scene);
	cameraObject.transform.LocalTranslate({ 0.0f, 1.0f, -10.0f });
	Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
	camera.SetType(ORTHOGRAPHIC); // PERSPECTIVE  ORTHOGRAPHIC
	camera.orthographic.viewWidth = 10.0f;
	camera.orthographic.viewHeight = 10.0f;
	camera.orthographic.farPlane = 500.0f;
	camera.orthographic.nearPlane = 0.01f;
	cameraObject.SetName("Camera");

	// ========== SECTION 1: TEST CIRCLE-CIRCLE COLLISION ==========

	GameObject& staticCircle = GameObject::Create(scene);
	staticCircle.AddScript<TranslateScript>();
	MeshRenderer& meshRendererStaticCircle = *staticCircle.AddComponent<MeshRenderer>();
	meshRendererStaticCircle.SetGeometry(SHAPES.SPHERE);
	staticCircle.SetName("StaticCircle");

	staticCircle.transform.SetLocalPosition(Vector3f32(-3.0f, 2.0f, 0.0f));
	staticCircle.transform.LocalScale(Vector3f32(0.5f, 0.5f, 0.5f));
	CircleCollider2D& staticCircleCollider = *staticCircle.AddComponent<CircleCollider2D>();

	GameObject& dynamicCircle = GameObject::Create(scene);
	dynamicCircle.AddScript<SetPositionScript>();
	MeshRenderer& meshRendererDynamicCircle = *dynamicCircle.AddComponent<MeshRenderer>();
	meshRendererDynamicCircle.SetGeometry(SHAPES.SPHERE);
	dynamicCircle.transform.SetLocalPosition(Vector3f32(-3.0f, 4.0f, 0.0f));
	dynamicCircle.transform.LocalScale(Vector3f32(0.4f, 0.4f, 0.4f));
	CircleCollider2D& dynamicCircleCollider = *dynamicCircle.AddComponent<CircleCollider2D>();
	PhysicComponent& dynamicCirclePhysic = *dynamicCircle.AddComponent<PhysicComponent>();
	dynamicCirclePhysic.SetMass(1.0f);
	dynamicCirclePhysic.SetBounciness(0.7f);
	dynamicCircle.SetName("DynamicCircle");

	// ========== SECTION 2: TEST BOX-BOX COLLISION ========== ERROR SCALE CUBES

	GameObject& staticBox = GameObject::Create(scene);
	MeshRenderer& meshRendererStaticBox = *staticBox.AddComponent<MeshRenderer>();
	meshRendererStaticBox.SetGeometry(SHAPES.CUBE);
	staticBox.transform.SetLocalPosition(Vector3f32(3.0f, -1.0f, 0.0f));
	staticBox.transform.LocalScale(Vector3f32(0.8f, 0.2f, 0.5f));
	BoxCollider2D& staticBoxCollider = *staticBox.AddComponent<BoxCollider2D>();
	staticBox.SetName("StaticBox");

	GameObject& dynamicBox = GameObject::Create(scene);
	dynamicBox.AddScript<ResetDynamicBox>();
	MeshRenderer& meshRendererDynamicBox = *dynamicBox.AddComponent<MeshRenderer>();
	meshRendererDynamicBox.SetGeometry(SHAPES.CUBE);
	dynamicBox.transform.SetLocalPosition(Vector3f32(3.0f, 3.0f, 0.0f));
	dynamicBox.transform.LocalScale(Vector3f32(0.5f, 0.5f, 0.5f));
	BoxCollider2D& dynamicBoxCollider = *dynamicBox.AddComponent<BoxCollider2D>();
	PhysicComponent& dynamicBoxPhysic = *dynamicBox.AddComponent<PhysicComponent>();
	dynamicBoxPhysic.SetMass(2.0f);
	dynamicBoxPhysic.SetBounciness(0.5f);
	dynamicBox.SetName("DynamicBox");

	// ========== SECTION 3: TEST CIRCLE-BOX COLLISION ==========

	GameObject& platformBox = GameObject::Create(scene);
	MeshRenderer& meshRendererPlatformBox = *platformBox.AddComponent<MeshRenderer>();
	meshRendererPlatformBox.SetGeometry(SHAPES.CUBE);
	platformBox.transform.SetLocalPosition(Vector3f32(0.0f, -2.0f, 0.0f));
	platformBox.transform.LocalScale(Vector3f32(4.0f, 0.2f, 0.5f));
	BoxCollider2D& platformBoxCollider = *platformBox.AddComponent<BoxCollider2D>();
	platformBox.SetName("PlatformBox");


	GameObject& bouncingCircle = GameObject::Create(scene);
	bouncingCircle.AddScript<BouncingScript>();
	MeshRenderer& meshRendererBouncingCircle = *bouncingCircle.AddComponent<MeshRenderer>();
	meshRendererBouncingCircle.SetGeometry(SHAPES.CUBE);
	bouncingCircle.transform.SetLocalPosition(Vector3f32(0.0f, 1.0f, 0.0f));
	bouncingCircle.transform.LocalScale(Vector3f32(0.6f, 0.6f, 0.6f));
	CircleCollider2D& bouncingCircleCollider = *bouncingCircle.AddComponent<CircleCollider2D>();
	PhysicComponent& bouncingCirclePhysic = *bouncingCircle.AddComponent<PhysicComponent>();
	bouncingCirclePhysic.SetMass(1.0f);
	bouncingCirclePhysic.SetBounciness(0.8f);
	bouncingCircle.SetName("BouncingCircle");

	GameObject& heavyCircle = GameObject::Create(scene);
	MeshRenderer& meshRendererHeavyCircle = *heavyCircle.AddComponent<MeshRenderer>();
	meshRendererHeavyCircle.SetGeometry(SHAPES.SPHERE);
	heavyCircle.transform.SetLocalPosition(Vector3f32(2.0f, 1.0f, 0.0f));
	heavyCircle.transform.LocalScale(Vector3f32(0.7f, 0.7f, 0.7f));
	CircleCollider2D& heavyCircleCollider = *heavyCircle.AddComponent<CircleCollider2D>();
	PhysicComponent& heavyCirclePhysic = *heavyCircle.AddComponent<PhysicComponent>();
	heavyCirclePhysic.SetMass(10.0f);
	heavyCirclePhysic.SetBounciness(0.3f);
	heavyCircle.SetName("HeavyCircle");

	GameObject& lightCircle = GameObject::Create(scene);
	MeshRenderer& meshRendererLightCircle = *lightCircle.AddComponent<MeshRenderer>();
	meshRendererLightCircle.SetGeometry(SHAPES.SPHERE);
	lightCircle.transform.SetLocalPosition(Vector3f32(1.0f, 1.0f, 0.0f));
	lightCircle.transform.LocalScale(Vector3f32(0.4f, 0.4f, 0.4f));
	CircleCollider2D& lightCircleCollider = *lightCircle.AddComponent<CircleCollider2D>();
	PhysicComponent& lightCirclePhysic = *lightCircle.AddComponent<PhysicComponent>();
	lightCirclePhysic.SetMass(0.5f);
	lightCirclePhysic.SetBounciness(0.9f);
	lightCircle.SetName("LightCircle");

	// ========== SECTION 4: OBJET WITH FORCES ==========

	GameObject& forcedCircle = GameObject::Create(scene);
	forcedCircle.AddScript<PhysicScript>();
	MeshRenderer& meshRendererForcedCircle = *forcedCircle.AddComponent<MeshRenderer>();
	meshRendererForcedCircle.SetGeometry(SHAPES.SPHERE);
	forcedCircle.transform.SetLocalPosition(Vector3f32(-2.0f, 0.0f, 0.0f));
	forcedCircle.transform.LocalScale(Vector3f32(0.5f, 0.5f, 0.5f));
	CircleCollider2D& forcedCircleCollider = *forcedCircle.AddComponent<CircleCollider2D>();
	PhysicComponent& forcedCirclePhysic = *forcedCircle.AddComponent<PhysicComponent>();
	forcedCirclePhysic.SetMass(1.0f);
	forcedCirclePhysic.SetBounciness(0.5f);
	forcedCircle.SetName("ForcedCircle");

	// Constant Force
	Force constantForce;
	constantForce.direction = { 1.0f, 0.0f, 0.0f };
	constantForce.norm = 100.0f;
	forcedCirclePhysic.AddForce(constantForce);

	// ========== SECTION 5: WALL BOUNDARY ==========

	// Floor
	GameObject& floor = GameObject::Create(scene);
	MeshRenderer& meshRendererFloor = *floor.AddComponent<MeshRenderer>();
	meshRendererFloor.SetGeometry(SHAPES.CUBE);
	floor.transform.SetLocalPosition(Vector3f32(0.0f, -4.0f, 0.0f));
	floor.transform.LocalScale(Vector3f32(10.2f, 0.2f, 0.5f));
	BoxCollider2D& floorCollider = *floor.AddComponent<BoxCollider2D>();
	floor.SetName("Floor");

	// Left Wall
	GameObject& leftWall = GameObject::Create(scene);
	MeshRenderer& meshRendererLeftWall = *leftWall.AddComponent<MeshRenderer>();
	meshRendererLeftWall.SetGeometry(SHAPES.CUBE);
	leftWall.transform.SetLocalPosition(Vector3f32(-5.0f, 0.0f, 0.0f));
	leftWall.transform.LocalScale(Vector3f32(0.2f, 7.8f, 0.5f));
	BoxCollider2D& leftWallCollider = *leftWall.AddComponent<BoxCollider2D>();
	leftWall.SetName("LeftWall");

	// Right Wall
	GameObject& rightWall = GameObject::Create(scene);
	MeshRenderer& meshRendererRightWall = *rightWall.AddComponent<MeshRenderer>();
	meshRendererRightWall.SetGeometry(SHAPES.CUBE);
	rightWall.transform.SetLocalPosition(Vector3f32(5.0f, 0.0f, 0.0f));
	rightWall.transform.LocalScale(Vector3f32(0.2f, 7.8f, 0.5f));
	BoxCollider2D& rightWallCollider = *rightWall.AddComponent<BoxCollider2D>();
	rightWall.SetName("RightWall");

	GameManager::Run( windowParam );

	delete &pso;

	GameManager::Destroy();

END_TEST_MAIN_FUNCTION(Collider2D)

DECLARE_SCRIPT( TranslateScript, ScriptFlag::Update )
void Update() override
{
	if (GetKey(Keyboard::LEFT)) m_pOwner->transform.LocalTranslate({ -0.1f, 0.0f, 0.0f });
	if (GetKey(Keyboard::RIGHT)) m_pOwner->transform.LocalTranslate({ 0.1f, 0.0f, 0.0f });
	if (GetKey(Keyboard::UP)) m_pOwner->transform.LocalTranslate({ 0.0f, 0.1f, 0.0f });
	if (GetKey(Keyboard::DOWN)) m_pOwner->transform.LocalTranslate({ 0.0f, -0.1f, 0.0f });
}
END_SCRIPT

DECLARE_SCRIPT( SetPositionScript, ScriptFlag::Update )
void Update() override
{
	if (GetKeyDown(Keyboard::Q)) m_pOwner->transform.SetLocalPosition(Vector3f32(-3.0f, 4.0f, 0.0f));
}
END_SCRIPT

DECLARE_SCRIPT( ResetDynamicBox, ScriptFlag::Update )
void Update() override
{
	if (GetKeyDown(Keyboard::W))
		m_pOwner->transform.SetLocalPosition(Vector3f32(3.0f, 3.0f, 0.0f));
}
END_SCRIPT

DECLARE_SCRIPT( BouncingScript, ScriptFlag::Update | ScriptFlag::Collision2DEnter | ScriptFlag::Collision2DExit )
void Update() override
{
	if (GetKeyDown(Keyboard::SPACEBAR))
	{
		m_pOwner->transform.SetLocalPosition(Vector3f32(0.0f, 5.0f, 0.0f));
		PhysicComponent* physics = m_pOwner->GetComponent<PhysicComponent>();
		if (physics) physics->GetVelocity() = Vector3f32(0.0f, 0.0f, 0.0f);
	}
}
void Collision2DEnter(GameObject* pOther) override
{
	String name = m_pOwner->GetName();
	String otherName = pOther->GetName();
	Console::Log("Begin Overlap: " + name + " with " + otherName);
}
void Collision2DExit(GameObject* pOther) override
{
	String name = m_pOwner->GetName();
	String otherName = pOther->GetName();
	Console::Log("End Overlap: " + name + " with " + otherName);
}
END_SCRIPT

DECLARE_SCRIPT( PhysicScript, ScriptFlag::Update )
void Update() override
{
	if (GetKeyDown(Keyboard::R))
	{
		m_pOwner->transform.SetLocalPosition(Vector3f32(-2.0f, 0.0f, 0.0f));
		PhysicComponent* physics = m_pOwner->GetComponent<PhysicComponent>();
		if (physics)
		{
			physics->GetVelocity() = Vector3f32(0.0f, 0.0f, 0.0f);

			Force impulse;
			impulse.direction = Vector3f32(1.0f, 0.5f, 0.0f).Normalize();
			impulse.norm = 5.0f;
			physics->AddForce(impulse);
		}
	}
}
END_SCRIPT


END_TEST(Collider2D)


#endif