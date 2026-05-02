#ifndef TEST_STATE_MACHINE_HPP_INCLUDED
#define TEST_STATE_MACHINE_HPP_INCLUDED

#include <StateMachine.h>
#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;

void MovementTest(GameObject* const pGameObject, bool const global)
{

    if (pGameObject == nullptr) return;

    float32 const deltaTime = GameManager::DeltaTime();

    if (GetKey(Keyboard::Z))
        pGameObject->transform.LocalTranslate(pGameObject->transform.GetLocalForward() * deltaTime);
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

    if (GetKey(Keyboard::S))
        pGameObject->transform.LocalTranslate(-pGameObject->transform.GetLocalForward() * deltaTime);

    if (GetKey(Keyboard::Q))
        pGameObject->transform.LocalTranslate(-pGameObject->transform.GetLocalRight() * deltaTime);

    if (GetKey(Keyboard::D))
        pGameObject->transform.LocalTranslate(pGameObject->transform.GetLocalRight() * deltaTime);

    if (GetKey(Keyboard::SPACE))
        pGameObject->transform.LocalTranslate(pGameObject->transform.GetLocalUp() * deltaTime);

    if (GetKey(Keyboard::LCONTROL))
        pGameObject->transform.LocalTranslate(-pGameObject->transform.GetLocalUp() * deltaTime);
    Vector3f32 one;
    one.SetOne();
    scale += one;

    if (global)
    {
        pGameObject->transform.WorldTranslate(translation);
        pGameObject->transform.LocalScale(scale);
        return;
    }

    pGameObject->transform.LocalTranslate(translation);
    pGameObject->transform.LocalScale(scale);
}

// === Idle State Functions ===
void OnBeginIdle(GameObject* me) {

    Console::Log("[Idle] Begin");
}
void OnUpdateIdle(GameObject* me) {


    Console::Log("[Idle] Updating... Waiting for start request.");

}
void OnEndIdle(GameObject* me) { Console::Log("[Idle] End"); }

// === Following State Functions ===
void OnBeginFollowing(GameObject* me) {
    // Console::Log("[Following] Begin");
}
void OnUpdateFollowing(GameObject* me) {

    //  Console::Log("[Following] Updating... Chasing player");

    GameObject* player = me->GetScene()->GetGameObject(0);
    Vector3f32 enemyPos = me->transform.GetWorldPosition();
    Vector3f32 playerPos = player->transform.GetWorldPosition();

    Vector3f32 direction = playerPos - enemyPos;
    direction.SelfNormalize();

    float speed = 0.5f * GameManager::DeltaTime();
    Vector3f32 newPos = enemyPos + direction * speed;
    me->transform.SetWorldPosition(newPos);
}
void OnEndFollowing(GameObject* me) {
    // Console::Log("[Following] End");
}

// === Condition Function ===
bool IsPlayerNear(GameObject* me) {
    GameObject* player = me->GetScene()->GetGameObject(0);
    if (player == nullptr) return false;

    Vector3f32 DistVect = player->transform.GetLocalPosition() - me->transform.GetLocalPosition();
    float distance = DistVect.Norm();

    Console::Log("Distance entre joueur et ennemi : " + std::to_string(distance));

    return distance < 4.0f; // Seuil de distance
}

bool IsPlayerFar(GameObject* me) {
    GameObject* player = me->GetScene()->GetGameObject(0);
    if (player == nullptr) return false;

    float distance = (player->transform.GetLocalPosition() - me->transform.GetLocalPosition()).Norm();
    Console::Log("Distance entre joueur et ennemi (Far): " + std::to_string(distance));

    return distance > 10.0f; // Distance seuil
}

BEGIN_TEST( StateMachine )

    GameManager::Create();
    Console::Init();
    Scene& scene = Scene::Create();

    Geometry* const pCube = GeometryFactory::CreateCubeGeo();

    Texture Texture1("res/Textures/stone2_albedo.jpg");
    Texture Texture2("res/Textures/test1.png");

    GameObject& cameraObject = GameObject::Create(scene);
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetType(PERSPECTIVE);
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };
    cameraObject.transform.SetWorldPosition({ 0.0f,0.0f,-10.0f });


    GameObject& second = GameObject::Create(scene);

    second.transform.SetWorldPosition({ 0, 0, 0 });
    MeshRenderer& meshRenderer2 = *second.AddComponent<MeshRenderer>();
    meshRenderer2.SetGeometry(pCube);
    meshRenderer2.SetAlbedoTexture(&Texture1);


        for (int i = 0; i < 5; i++)
        {
            GameObject& third = GameObject::Create(scene);

            third.transform.SetWorldPosition({ float(i),float(-i), 0 });
            third.transform.SetLocalScale({ 0.5, 0.5, 0.5 });

            MeshRenderer& meshRenderer3 = *third.AddComponent<MeshRenderer>();
            meshRenderer3.SetGeometry(pCube);

            meshRenderer3.SetAlbedoTexture(&Texture2);


            StateMachine* sm = GameManager::GetStatesSystem().CreateStateMachine(&third);

            // Add Idle state
            String idle = "Idle";
            sm->AddAction(idle, OnBeginIdle, OnUpdateIdle, OnEndIdle);

            // Add Following state
            String following = "Following";
            sm->AddAction(following, OnBeginFollowing, OnUpdateFollowing, OnEndFollowing);

            // Add transition Idle → Following
            StateMachine::Condition nearCondition = { IsPlayerNear };
            Vector<StateMachine::Condition> nearConditions;
            nearConditions.PushBack(nearCondition);
            //sm.AddTransition(nearConditions, following);


            // transition Following → Idle
            StateMachine::Condition farCondition = { IsPlayerFar };
            Vector<StateMachine::Condition> farConditions;
            farConditions.PushBack(farCondition);
            //sm.AddTransition(farConditions, idle);
        }

    WindowParam windowParam;

    GameManager::Run(windowParam);

    delete pCube;
    GameManager::Destroy();
    

END_TEST_MAIN_FUNCTION(StateMachine)
END_TEST( StateMachine )


#endif