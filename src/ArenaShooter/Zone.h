#pragma once
#include "define.h"
#include <Engine.h>

struct ObjectReach
{
	gce::GameObject* pgameObject;
	bool obstructed;
};

DECLARE_SCRIPT(ZoneScript, ScriptFlag::CollisionEnter | ScriptFlag::Update)

int m_inactiveAmount = 0;

bool m_cleared = false;
bool m_tookAggro = false;
gce::Vector<gce::GameObject*> EnemyInZone;

void Reset();
void EnemyDeath();

ObjectReach* GetClosestEnemy(gce::Vector3f32 position, gce::GameObject* _exception = nullptr);

void CollisionEnter(gce::GameObject* other);
void Update();



END_SCRIPT
