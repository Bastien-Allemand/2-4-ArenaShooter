#pragma once
#include "define.h"
#include <Engine.h>



class Menu
{
public:
	enum MenuType
	{
		MAIN,
		GAMEOVER,
		PAUSE,
		WIN,
		NONE
	};

	MenuType m_active;

	Menu();
	
	void InitMenu();
	void SetActive(MenuType activeMenu);
	void Deactivate();
private:

	gce::BitMapBrush* m_mainQuit;
	gce::BitMapBrush* m_newGame;
	gce::BitMapBrush* m_settings;
	gce::UiButton* m_quitButton;
	gce::UiButton* m_newGameButton;
	gce::UiButton* m_settingsButton;

	gce::BitMapBrush* m_retry;
	gce::BitMapBrush* m_menu;
	gce::BitMapBrush* m_partyOver;
	gce::UiButton* m_retryButton;
	gce::UiButton* m_menuButton;

	gce::BitMapBrush* m_continueAlt;
	gce::BitMapBrush* m_save;
	gce::BitMapBrush* m_pause;
	gce::BitMapBrush* m_quit;
	gce::UiButton* m_continueAltButton;
	gce::UiButton* m_saveButton;
	gce::UiButton* m_pauseToMenuButton;

	std::vector<gce::GameObject*> m_mainMenuObjects;
	std::vector<gce::GameObject*> m_gameOverObjects;
	std::vector<gce::GameObject*> m_pauseObjects;
	std::vector<gce::GameObject*> m_winObjects;
	
};

