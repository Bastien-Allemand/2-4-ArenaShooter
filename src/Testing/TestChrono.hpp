#ifndef TEST_CHRONO_HPP_INCLUDED
#define TEST_CHRONO_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST(Chrono)

Console::Init();

//////////////////////////////////////////////////////////////////
/// c1 and c2 two Chronos.
/// Console logs the elapsed time every frame for each Chrono.
/// Pressing the 'f' key will pause c1.
/// Pressing the 'g' key will pause c2.
/// Pressing the 'r' key will start c1.
/// Pressing the 't' key will start c2.
/// Pressing the 'v' key will reset c1.
/// Pressing the 'b' key will reset c2.
/// Pressing 'Escape' will exit the test.
//////////////////////////////////////////////////////////////////

GameManager::Create();

Scene& scene = Scene::Create();

GameObject& testManager = GameObject::Create(scene);
testManager.AddScript<ChronoTestManager>();

WindowParam wp;
GameManager::Run(wp);

GameManager::Destroy();


END_TEST_MAIN_FUNCTION(Chrono)

DECLARE_SCRIPT(ChronoTestManager, ScriptFlag::Update)

Chrono c1;
Chrono c2;

void Update()
{
	if (GetKeyDown(Keyboard::F))
		c1.Pause();
	if (GetKeyDown(Keyboard::G))
		c2.Pause();

	if (GetKeyDown(Keyboard::R))
		c1.Start();
	if (GetKeyDown(Keyboard::T))
		c2.Start();

	if (GetKeyDown(Keyboard::V))
		Console::Log("[c1]: RESETTING " + std::to_string(c1.Reset()));
	if (GetKeyDown(Keyboard::B))
		Console::Log("[c2]: RESETTING " + std::to_string(c2.Reset()));

	Console::Log("[c1]: " + std::to_string(c1.GetElapsedTime()) + "  |  [c2]: " + std::to_string(c2.GetElapsedTime()));
}

END_SCRIPT

END_TEST(Chrono)

#endif