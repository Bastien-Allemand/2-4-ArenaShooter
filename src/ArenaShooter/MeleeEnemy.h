#pragma once
#include "BotEnemy.h"

DECLARE_CHILD_SCRIPT(MeleeEnemy, BotEnemy, ScriptFlag::Start)

gce::Vector3f32 m_targetDirection;

gce::GameObject* mp_attackArea;

float m_distanceTravelled;
float m_distanceToNext;

// Attack state
const float m_attackTime = 1.0f;
float m_attackChrono = 0.f;
float m_dashSpeed;

// Telegraph state
const float m_telegraphTime = 1.0f;
float m_telegraphChrono = 0.f;


void Start();

END_SCRIPT