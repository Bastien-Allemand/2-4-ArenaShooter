#include "ArenaManager.h"
#include <Engine.h>
#include <Engine/StateMachine.h>
#include "PlayerController.h"
#include "MapLoader.h"

ArenaManager* ArenaManager::mp_instance = nullptr;
ItemManager* ArenaManager::mp_itemManager = nullptr;
ResourceManager* ArenaManager::mp_resourceManager = nullptr;
Maploader* ArenaManager::mp_mapLoader = nullptr;

void MainMenuStart(gce::GameObject* pMe)
{
    gce::ShowMouseCursor();
    gce::GameManager::Pause();
    Menu* menu = ArenaManager::GetInstance()->GetMenu();
    menu->SetActive(Menu::MAIN);
}

void MainMenuExit(gce::GameObject* pMe)
{
    ArenaManager* script = ArenaManager::GetInstance();
    
    // Call every reset 
    script->GetItemManager()->Reset();
    script->GetPlayerData()->Reset();
    // TODO: Reset map

    script->GetPlayer()->GetScript<PlayerController>()->InitializeCam();

    ArenaManager* arenaManager = ArenaManager::GetInstance();
    ArenaManager::GetInstance()->LoadMap(0);

    gce::GameManager::Unpause();

    Menu* menu = arenaManager->GetMenu();
    menu->Deactivate();
}

void GameOverStart(gce::GameObject* pMe)
{
    gce::ShowMouseCursor();
    gce::GameManager::Pause();
    ArenaManager* script = ArenaManager::GetInstance();
    Menu* menu = script->GetMenu();
    menu->SetActive(Menu::GAMEOVER);
}

void WinStart(gce::GameObject* pMe)
{
    gce::GameManager::Pause();
    ArenaManager* script = ArenaManager::GetInstance();
    Menu* menu = script->GetMenu();
    menu->SetActive(Menu::WIN);
}

void PlayingStart(gce::GameObject* pMe)
{
    gce::HideMouseCursor();
    ArenaManager* script = ArenaManager::GetInstance();

    gce::GameObject* player = script->GetPlayer();
    player->SetActive(true);
    script->GetPlayerData()->ActivateUI(true);
    script->GetItemManager()->ActivateUi(true);

    int width = gce::GameManager::GetWindow()->GetWidth();
    int height = gce::GameManager::GetWindow()->GetHeight();
    gce::SetMousePosition({ width / 2, height / 2 });

    gce::GameManager::Unpause();
}

void PlayingExit(gce::GameObject* pMe)
{
    ArenaManager* script = ArenaManager::GetInstance();
    script->GetPlayer()->SetActive(false);
    script->GetPlayerData()->ActivateUI(false);
    script->GetItemManager()->ActivateUi(false);
}

void PauseStart(gce::GameObject* pMe)
{
    gce::ShowMouseCursor();
    gce::GameManager::Pause();
    Menu* menu = ArenaManager::GetInstance()->GetMenu();
    menu->SetActive(Menu::PAUSE);
}

void PauseExit(gce::GameObject* pMe)
{
    Menu* menu = ArenaManager::GetInstance()->GetMenu();
    menu->Deactivate();
}

void ArenaManager::Init()
{
    srand(timeGetTime());

    GetResourceManager()->Init();

    mp_activeScene = &gce::Scene::Create();

    mp_mainMenu = new Menu();
    mp_mainMenu->InitMenu();

    mp_stateMachine = gce::GameManager::GetStatesSystem().CreateStateMachine(&gce::GameObject::Create(*mp_activeScene));

    mp_stateMachine->AddAction("Playing", PlayingStart, nullptr, PlayingExit);
    mp_stateMachine->AddAction("Pause", PauseStart, nullptr, PauseExit);
    mp_stateMachine->AddAction("MainMenu", MainMenuStart, nullptr, MainMenuExit);
    mp_stateMachine->AddAction("GameOver", GameOverStart, nullptr, nullptr);
    mp_stateMachine->AddAction("Win",WinStart, nullptr, nullptr);
    mp_stateMachine->Transit("MainMenu");

    mp_mapLoader = ArenaManager::GetMapLoader();
    mp_mapLoader->Init();
}

ArenaManager* ArenaManager::GetInstance()
{
    if (mp_instance == nullptr)
    {
        mp_instance = new ArenaManager();
    }
    return mp_instance;
}

ItemManager* ArenaManager::GetItemManager()
{
    if (mp_itemManager == nullptr)
    {
        mp_itemManager = GetInstance()->GetPlayer()->AddScript<ItemManager>();
    }
    return mp_itemManager;
}

ResourceManager* ArenaManager::GetResourceManager()
{
    if (mp_resourceManager == nullptr)
    {
        mp_resourceManager = new ResourceManager();
    }
    return mp_resourceManager;
}

gce::GameObject* ArenaManager::GetPlayer()
{
    if (mp_player == nullptr)
    {
        mp_player = &gce::GameObject::Create(*mp_activeScene);

        // Camera
        gce::GameObject* cameraObject = &gce::GameObject::Create(*mp_activeScene);
        mp_playerData = mp_player->AddScript<Player>();
        mp_camera = cameraObject->AddComponent<gce::Camera>();
        gce::SetMousePosition(gce::Vector2i32(640, 360));
        mp_camera->SetMainCamera();
        mp_camera->SetType(gce::PERSPECTIVE);
        mp_camera->perspective.fov = XM_PIDIV4;
        mp_camera->perspective.nearPlane = 0.001f;
        mp_camera->perspective.farPlane = 500.0f;
        mp_camera->perspective.aspectRatio = 600.0f / 400.0f;
        mp_camera->perspective.up = { 0.0f, 1.0f, 0.0f };
        mp_player->AddChild(*cameraObject);
        cameraObject->transform.SetLocalPosition({ 0, 0.9f, 0 });

        // Physics
        gce::BoxCollider* playerBox = mp_player->AddComponent<gce::BoxCollider>();
        playerBox->localScale = { 1, 1.8f, 1 };
        gce::PhysicComponent* playerPhysics = mp_player->AddComponent<gce::PhysicComponent>();
        playerPhysics->SetBounciness(0);
        playerPhysics->SetGravityScale(5);

        // Gun
        gce::GameObject& playerGun = gce::GameObject::Create(*mp_activeScene);
        gce::MeshRenderer& meshRendererGun = *playerGun.AddComponent<gce::MeshRenderer>();
        meshRendererGun.SetGeometry(gce::GeometryFactory::LoadGeometry("res/Models/D_eagle.obj"));
        meshRendererGun.SetAlbedoTexture(ResourceManager::GetTexture("res/Textures/texture_D_eagle.png"));
        cameraObject->AddChild(playerGun);
        playerGun.transform.SetLocalPosition(gce::Vector3f32(0.35f, -0.45f, 0.8f));
        
        // Scripts
        PlayerController* playerScript = mp_player->AddScript<PlayerController>();
        playerScript->mp_playerCamera = cameraObject;
        playerScript->mp_playerGun = &playerGun;

        mp_player->AddScript<ItemManager>();
    }

    return mp_player;
}

gce::GameObject* ArenaManager::GetClosestEnemy(gce::Vector3f32 position, gce::GameObject* _exception) {
    gce::GameObject* closestEnemy = nullptr;
    float minimumDelta = INFINITE;
    for (auto enemy : mp_enemyList) {
        if (enemy == _exception || enemy->IsActive() == false)
        {
            continue;
        }

        if ((enemy->transform.GetWorldPosition() - position).Norm() < minimumDelta) {
            minimumDelta = (enemy->transform.GetWorldPosition() - position).Norm();
            closestEnemy = enemy;
        }
    }
    return closestEnemy;
}

Maploader* ArenaManager::GetMapLoader()
{
    if (mp_mapLoader == nullptr)
    {
        mp_mapLoader = new Maploader();
    }
    return mp_mapLoader;
}

void ArenaManager::AddEnemy(gce::GameObject* enemy){
    mp_enemyList.push_back(enemy);
}

void ArenaManager::SetState(String state)
{
    mp_stateMachine->Transit(state);
}

void ArenaManager::LoadMap(int level)
{
    mp_mapLoader->loadMap(level);
    mp_enemyList.clear();
}
