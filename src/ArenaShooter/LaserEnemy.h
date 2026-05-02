#pragma once
#include "BotEnemy.h"

DECLARE_CHILD_SCRIPT(LaserEnemy, BotEnemy, ScriptFlag::Start)

gce::Vector3f32 m_targetDirection;

float m_distanceTravelled;
float m_distanceToNext;

// Attack state
const float m_attackTime = 0.65f;
float m_attackChrono = 0.f;

// Telegraph state
const float m_telegraphTime = 0.2f;
float m_telegraphChrono = 0.f;

void Start();

END_SCRIPT