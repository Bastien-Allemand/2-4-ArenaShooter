#include "Menu.h"
#include "ArenaManager.h"
#include "PlayerController.h"
#include "GameUtils.h"
#include <Engine/Components/UIButton.h>
#include "Player.h"
#include "ItemManager.h"

void CallStart()
{
	ArenaManager::GetInstance()->SetState("Playing");
	return;
}

void Exit()
{
	exit(0);
	return;
}

void Retry()
{
	ArenaManager::GetInstance()->SetState("MainMenu");
	ArenaManager::GetInstance()->SetState("Playing");
	return;
}

void CallMainMenu()
{
	ArenaManager::GetInstance()->SetState("MainMenu");
	return;
}

void ResumeGame()
{
	ArenaManager::GetInstance()->SetState("Playing");
	return;
}

void CallGameOver()
{
	ArenaManager::GetInstance()->SetState("GameOver");
	return;
}

void CallPause()
{
	ArenaManager::GetInstance()->SetState("Pause");
	return;
}


Menu::Menu()
{
	
}

void Menu::InitMenu()
{
	ArenaManager::GetInstance()->GetPlayer()->GetScript<Player>()->ActivateUI(false);
	ArenaManager::GetInstance()->GetItemManager()->ActivateUi(false);
	gce::Vector2f32 center = { 1920 / 2.f, 1060 / 2.f };
	gce::Vector2f32 size = { 533 ,782 };
	gce::Vector2f32 posUi = center - size * 0.5f;

#pragma region Main
	m_mainQuit = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/bouton_quit_mainmenu.png");
	m_newGame = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/bouton_new_game.png");
	m_settings = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/bouton_settings.png");

	m_mainMenuObjects.push_back(&CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/MainBackground.png"), { 0, 0 }, { 1, 1 }, { 0, 0 }));

	m_mainMenuObjects.push_back(&gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()));
	m_newGameButton = m_mainMenuObjects[1]->AddComponent<gce::UiButton>();
	m_newGameButton->pBitMapBrush = m_newGame;
	m_newGameButton->pHoverBitMapBrush = m_newGame;
	m_newGameButton->pBitMapBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 122,45,0 }, 0);
	m_newGameButton->SetActive(true);
	m_newGameButton->AddListener(CallStart);
	m_mainMenuObjects[1]->transform.SetWorldPosition({ center.x - 5, center.y + 75, 0 });
	m_mainMenuObjects[1]->transform.SetWorldScale({ 300,100,1 });

	m_mainMenuObjects.push_back(&gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()));
	m_settingsButton = m_mainMenuObjects[2]->AddComponent<gce::UiButton>();
	m_settingsButton->pBitMapBrush = m_settings;
	m_settingsButton->pHoverBitMapBrush = m_settings;
	m_settingsButton->pBitMapBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 122,45,0 }, 0);
	m_settingsButton->SetActive(true);
	/*m_settingsButton->AddListener(CallPause);*/
	m_mainMenuObjects[2]->transform.SetWorldPosition({ center.x - 5, center.y + 200, 0 });
	m_mainMenuObjects[2]->transform.SetWorldScale({ 300,100,1 });

	m_mainMenuObjects.push_back(&gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()));
	m_quitButton = m_mainMenuObjects[3]->AddComponent<gce::UiButton>();
	m_quitButton->pBitMapBrush = m_mainQuit;
	m_quitButton->pHoverBitMapBrush = m_mainQuit;
	m_quitButton->pBitMapBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 122,45,0 }, 0);
	m_quitButton->SetActive(true);
	m_quitButton->AddListener(Exit);
	m_mainMenuObjects[3]->transform.SetWorldPosition({ center.x - 5, center.y + 325, 0 });
	m_mainMenuObjects[3]->transform.SetWorldScale({ 300,100,1 });

	m_mainMenuObjects.push_back(&CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/MainTitle.png"), { center.x, 100 }, { 3, 3 }, { 0.5f, 0 }));
#pragma endregion

#pragma region GameOver

	m_retry = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/bouton_retry.png");
	m_menu = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/bouton_menu.png");
	m_partyOver = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/party_is_over.png");

	m_gameOverObjects.push_back(&gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()));
	m_retryButton = m_gameOverObjects[0]->AddComponent<gce::UiButton>();
	m_retryButton->pBitMapBrush = m_retry;
	m_retryButton->pHoverBitMapBrush = m_retry;
	m_retryButton->pBitMapBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 122,45,0 }, 0);
	m_retryButton->SetActive(true);
	m_retryButton->AddListener(Retry);
	m_gameOverObjects[0]->transform.SetWorldPosition({ center.x, center.y - 25 , 0 });
	m_gameOverObjects[0]->transform.SetWorldScale({ 300,100,1 });

	m_gameOverObjects.push_back(&gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()));
	m_menuButton = m_gameOverObjects[1]->AddComponent<gce::UiButton>();
	m_menuButton->pBitMapBrush = m_menu;
	m_menuButton->pHoverBitMapBrush = m_menu;
	m_menuButton->pBitMapBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 122,45,0 }, 0);
	m_menuButton->SetActive(true);
	m_menuButton->AddListener(CallMainMenu);
	m_gameOverObjects[1]->transform.SetWorldPosition({ center.x, center.y + 100, 0 });
	m_gameOverObjects[1]->transform.SetWorldScale({ 300, 100, 1 });

	m_gameOverObjects.push_back(&CreateImage(m_partyOver, { center.x ,center.y - 250 }, { 3,3 }));

#pragma endregion

#pragma region Pause
	m_continueAlt = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/bouton_continue_sans_plaque.png");
	m_pause = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/cadre_pause.png");
	m_quit = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/bouton_menu_pausegame.png");

	m_pauseObjects.push_back(&gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()));
	m_continueAltButton = m_pauseObjects[0]->AddComponent<gce::UiButton>();
	m_continueAltButton->pBitMapBrush = m_continueAlt;
	m_continueAltButton->pHoverBitMapBrush = m_continueAlt;
	m_continueAltButton->pBitMapBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 122,45,0 }, 0);
	m_continueAltButton->SetActive(true);
	m_continueAltButton->AddListener(ResumeGame);
	m_pauseObjects[0]->transform.SetWorldPosition({ center.x, center.y - 50 , 0 });
	m_pauseObjects[0]->transform.SetWorldScale({ 300, 100, 1 });

	//m_pauseObjects.push_back(&gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()));
	//m_saveButton = m_pauseObjects[1]->AddComponent<gce::UiButton>();
	//m_saveButton->pBitMapBrush = m_save;
	//m_saveButton->pHoverBitMapBrush = m_save;
	//m_saveButton->pBitMapBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 122,45,0 }, 0);
	//m_saveButton->SetActive(true);
	//m_pauseObjects[1]->transform.SetWorldPosition({ center.x, center.y + 125, 0 });
	//m_pauseObjects[1]->transform.SetWorldScale({ 300, 100, 1 });

	m_pauseObjects.push_back(&gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()));
	m_pauseToMenuButton = m_pauseObjects[1]->AddComponent<gce::UiButton>();
	m_pauseToMenuButton->pBitMapBrush = m_quit;
	m_pauseToMenuButton->pHoverBitMapBrush = m_quit;
	m_pauseToMenuButton->pBitMapBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 122,45,0 }, 0);
	m_pauseToMenuButton->SetActive(true);
	m_pauseToMenuButton->AddListener(CallMainMenu);
	m_pauseObjects[1]->transform.SetWorldPosition({ center.x, center.y + 100, 0 });
	m_pauseObjects[1]->transform.SetWorldScale({ 300, 100, 1 });

	m_pauseObjects.push_back(&CreateImage(m_pause, { center.x ,center.y - 275 }, { 3,3 }));
#pragma endregion

	m_winObjects.push_back(&CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/Voctoire.png"), { center.x ,center.y - 70 }, { 3,3 }));
	m_winObjects.push_back(m_gameOverObjects[1]);

	for (auto object : m_mainMenuObjects)
	{
		object->SetActive(false);
	}
	for (auto object : m_gameOverObjects)
	{
		object->SetActive(false);
	}
	for( auto object : m_pauseObjects)
	{
		object->SetActive(false);
	}
	for (auto object : m_winObjects)
	{
		object->SetActive(false);
	}

	m_active = NONE;
}

void Menu::Deactivate()
{
	std::vector<gce::GameObject*>* objectsToDeactivate = nullptr;
	switch (m_active)
	{
	case MAIN:
		objectsToDeactivate = &m_mainMenuObjects;
		break;

	case GAMEOVER:
		objectsToDeactivate = &m_gameOverObjects;
		break;

	case PAUSE:

		objectsToDeactivate = &m_pauseObjects;
		break;
	case WIN:

		objectsToDeactivate = &m_winObjects;
		break;
	}
	if (objectsToDeactivate != nullptr)
	{
		for (auto object : *objectsToDeactivate)
		{
			object->SetActive(false);
		}
	}
	m_active = NONE;
}

void Menu::SetActive(MenuType activeMenu)
{
	std::vector<gce::GameObject*>* objectsToActivate = nullptr;
	std::vector<gce::GameObject*>* objectsToDeactivate = nullptr;

	switch (m_active)
	{
	case MAIN:
		objectsToDeactivate = &m_mainMenuObjects;
		break;

	case GAMEOVER:
		objectsToDeactivate = &m_gameOverObjects;
		break;
	case PAUSE:

		objectsToDeactivate = &m_pauseObjects;
		break;
	case WIN:

		objectsToDeactivate = &m_winObjects;
		break;
	}

	switch (activeMenu)
	{
	case MAIN:
		objectsToActivate = &m_mainMenuObjects;
		break;

	case GAMEOVER:
		objectsToActivate = &m_gameOverObjects;
		break;
	case PAUSE:

		objectsToActivate = &m_pauseObjects;
		break;
	case WIN:

		objectsToActivate = &m_winObjects;
		break;
	}
	if (objectsToActivate == objectsToDeactivate)
	{
		return;
	}

	if(objectsToDeactivate != nullptr)
	{
		for (auto object : *objectsToDeactivate)
		{
			object->SetActive(false);
		}
	}
	if (objectsToActivate != nullptr)
	{
		for (auto object : *objectsToActivate)
		{
			object->SetActive(true);
		}
	}
	m_active = activeMenu;
}