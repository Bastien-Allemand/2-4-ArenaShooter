#pragma once
#include "define.h"
#include <Engine.h>
class Maploader;

DECLARE_SCRIPT(ElevatorScript, ScriptFlag::Update | ScriptFlag::CollisionStay)

Maploader* m_mapLoader;
bool locked = false;

void Update();
void CollisionStay(gce::GameObject& other);

END_SCRIPT

