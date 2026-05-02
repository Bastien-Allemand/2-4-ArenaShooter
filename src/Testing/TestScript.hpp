#ifndef TESTING_SCRIPTTEST_H_INCLUDED
#define TESTING_SCRIPTTEST_H_INCLUDED

#include <Script.h>
#include "define.h"
//#include "Tests.h"
#include <Engine.h>
#include <Render.h>
#include <InputsEnums.h>
#include <InputsMethods.h>
#include <Shapes.h>

using namespace gce;

BEGIN_TEST(Script)
	Console::Init();

	GameManager::Create();
	Scene& scene = Scene::Create();
	WindowParam windowParam;

	GameObject& cameraObject = GameObject::Create(scene);
	cameraObject.transform.LocalTranslate({ 0, 0, -10 });
	Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
	camera.SetType(PERSPECTIVE);
	camera.perspective.fov = XM_PIDIV4;
	camera.perspective.nearPlane = 0.001f;
	camera.perspective.farPlane = 500.0f;
	camera.perspective.up = { 0.0f, 1.0f, 0.0f };

	///////////////////////////////////////////////////////////////////
	/// 2 Objects, A and B
	/// ---
	/// A has TestScript1 and TestScript2
	/// A moves with ZQSD.
	/// Pressing '+' or '-' increases/decreases A's movement speed.
	/// Pressing backspace destroys A and sends a console log.
	/// When A collides, console logs are sent.
	/// ---
	/// B has TestScript3 (child of TestScript2)
	/// B rotates on update.
	/// When B exits collision with A, TestScript3 is removed.
	///////////////////////////////////////////////////////////////////

	GameObject& objectA = GameObject::Create(scene);
	TestScript1* pTs = objectA.AddScript<TestScript1>();
	TestScript2* pTs2 = objectA.AddScript<TestScript2>();
	objectA.transform.SetWorldPosition(Vector3f32(1, 0, 0));
	objectA.AddComponent<PhysicComponent>()->SetGravityScale(0);
	objectA.AddComponent<BoxCollider>();

	GameObject& objectB = GameObject::Create(scene);
	TestScript3* pTs3 = objectB.AddScript<TestScript3>();
	objectB.transform.SetWorldPosition(Vector3f32(0, -2, 0));
	objectB.AddComponent<PhysicComponent>()->SetGravityScale(0);
	objectB.AddComponent<BoxCollider>();

	MeshRenderer* meshRendererA = objectA.AddComponent<MeshRenderer>();
	meshRendererA->SetGeometry(SHAPES.CUBE);

	MeshRenderer* meshRendererB = objectB.AddComponent<MeshRenderer>();
	meshRendererB->SetGeometry(SHAPES.CUBE);

	GameManager::Run(windowParam);
	GameManager::Destroy();

	
	END_TEST_MAIN_FUNCTION()

	DECLARE_SCRIPT(TestScript1, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::FixedUpdate | ScriptFlag::CollisionStay | ScriptFlag::CollisionExit | ScriptFlag::CollisionEnter | ScriptFlag::Destroy)
	// Initialize speed (later used in FixedUpdate for movement)
	void Start() override
	{
		speed = 10.f;
		Console::Log("[TestScript1] Start has been called. 'speed' is: " + std::to_string(speed));
	}

	// User Input for movement
	void FixedUpdate() override
	{
		float32 fixedDeltaTime = GameManager::FixedDeltaTime();
		if (GetKey(Keyboard::Z))
		{
			m_pOwner->transform.LocalTranslate({ 0, speed * fixedDeltaTime, 0 });
		}
		if (GetKey(Keyboard::S))
		{
			m_pOwner->transform.LocalTranslate({ 0, -speed * fixedDeltaTime, 0 });
		}
		if (GetKey(Keyboard::Q))
		{
			m_pOwner->transform.LocalTranslate({ -speed * fixedDeltaTime, 0, 0 });
		}
		if (GetKey(Keyboard::D))
		{
			m_pOwner->transform.LocalTranslate({ speed * fixedDeltaTime, 0, 0 });
		}
		if (GetKey(Keyboard::E))
		{
			m_pOwner->transform.LocalRotate({ 0, 0, -speed * fixedDeltaTime });
		}
		if (GetKey(Keyboard::A))
		{
			m_pOwner->transform.LocalRotate({ 0, 0, speed * fixedDeltaTime });
		}
	}

	// User input: + or - to increase/decrease the 'speed' member and destroy self
	void Update() override
	{
		float32 deltaTime = GameManager::DeltaTime();

		if (GetKey(Keyboard::NUMPAD_ADD))
		{
			speed += deltaTime * 5;
		}

		if (GetKey(Keyboard::NUMPAD_SUBTRACT))
		{
			speed -= deltaTime * 5;
		}

		if (GetKey(Keyboard::BACKSPACE))
		{
			m_pOwner->Destroy();
		}
	}

	void Destroy() override
	{
		Console::Log("[TestScript1] Destroy has been called.");
	}

	void CollisionStay(GameObject* other) override
	{
		Console::Log("-COLLISION STAY- | " + String(other->GetName()));
	}

	void CollisionEnter(GameObject* other) override 
	{
		Console::Log("-----COLLISION ENTER----- | " + String(other->GetName()));
	}
	
	void CollisionExit(GameObject* other) override
	{
		Console::Log("-----COLLISION EXIT----- | " + String(other->GetName()));
		other->RemoveScript<TestScript3>();
	}

	float32 speed;

	END_SCRIPT

	DECLARE_SCRIPT(TestScript2, ScriptFlag::Start)
	// Initialize a member
	void Start()
	{
		health = 20;
		Console::Log("[TestScript2] Start has been called. 'health' is: " + std::to_string(health));
	}

	void Update()
	{
		Console::Log("[TestScript2] THIS METHOD SHOULD NOT BE CALLED, AS TESTSCRIPT2 SHOULD NOT HAVE THE UPDATE FLAG!", Console::RED);
	}

	int health;

	END_SCRIPT

	DECLARE_CHILD_SCRIPT(TestScript3, TestScript2, ScriptFlag::Start | ScriptFlag::Update)
	// Override parent initialize
	void Start() override
	{
		health = 21;
		Console::Log("[TestScript3] Start has been called. 'health' is: " + std::to_string(health));
	}

	// If rotating, the child flag addition works
	void Update() override
	{
		float32 deltaTime = GameManager::DeltaTime();
		m_pOwner->transform.LocalRotate({ 0, 0, deltaTime });
	}

	END_SCRIPT
END_TEST(Script)


#endif