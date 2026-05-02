#pragma once
#include "BotEnemy.h"

struct ObjectReach;

DECLARE_CHILD_SCRIPT(HealEnemy, BotEnemy, ScriptFlag::Start | ScriptFlag::Update| ScriptFlag::Destroy)

gce::Vector3f32 m_targetDirection;

ObjectReach* mp_healTarget;

gce::GameObject* mp_healVisual;

float m_distanceTravelled;
float m_distanceToNext;

float m_healChrono;
float m_healTime;

void Start();
void Update();
void Destroy();

END_SCRIPT