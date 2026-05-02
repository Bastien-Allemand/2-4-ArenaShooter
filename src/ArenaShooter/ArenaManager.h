#pragma once
#include <Engine.h>
#include <Render.h>
#include "ItemManager.h"
#include "ResourceManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Menu.h"
class Maploader;

class ArenaManager
{
	static ArenaManager* mp_instance;
	static ItemManager* mp_itemManager;
	static ResourceManager* mp_resourceManager;
	static Maploader* mp_mapLoader;

	gce::Scene* mp_activeScene;
	gce::GameObject* mp_player;
	gce::Camera* mp_camera;
	Player* mp_playerData;
	std::vector<gce::GameObject*> mp_enemyList;


	gce::StateMachine* mp_stateMachine;

	Menu* mp_mainMenu;

	ArenaManager() = default;
	~ArenaManager() = default;

public:
	void Init();

	static ArenaManager* GetInstance();
	static ItemManager* GetItemManager();
	static ResourceManager* GetResourceManager();
	static Maploader* GetMapLoader();
	void AddEnemy(gce::GameObject* enemy);
	gce::Scene* GetScene() { return mp_activeScene; };
	gce::GameObject* GetPlayer();
	Player* GetPlayerData() { return mp_playerData; };
	gce::Camera* GetCamera() { return mp_camera; };
	Menu* GetMenu() { return mp_mainMenu; };
	gce::GameObject* GetClosestEnemy(gce::Vector3f32 position, gce::GameObject* _exception = nullptr);
	void SetState(String state);
	void LoadMap(int level);
};

