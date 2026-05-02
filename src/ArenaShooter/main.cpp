#include "main.h"
#include <windows.h>
#include <Engine.h>
#include "define.h"
#include "ArenaManager.h"

#include "ItemCape.h"
#include "ItemApple.h"
#include "ItemCookie.h"
#include "ItemBirthdayCakeSlice.h"
#include "ItemCerealBar.h"
#include "ItemCoolWristband.h"
#include "ItemCrystalWings.h"
#include "ItemEnemyArmorPlate.h"
#include "ItemEnemyCoreGreen.h"
#include "ItemEnemyCoreOrange.h"
#include "ItemEnemyCoreRed.h"
#include "ItemEnemyJetpack.h"
#include "ItemKeyCard.h"
#include "ItemKey.h"
#include "ItemBlackFlask.h"



int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
	gce::GameManager::Create();

	ArenaManager::GetInstance()->Init();

	gce::WindowParam windowParam;
	windowParam.isFullScreen = true;

	gce::GameManager::Run(windowParam);

	gce::GameManager::Destroy();

    return 0;
}

