#pragma once
#include "Enemy.h"
#include <Engine.h>
struct ZoneScript;

DECLARE_CHILD_SCRIPT(BotEnemy, Enemy, ScriptFlag::Start | ScriptFlag::CollisionEnter)
	// Roam state
	const float m_roamTime = 1.0f;
	float m_roamChrono = 0.f;

	gce::Vector3f32 m_direction;

	bool roomRegistered = false;

	float attackActivationRange;
	ZoneScript* pZone;

	void Reset();
	void FindPathToAlly();
	virtual void Aggro();
	void Start();

	void CollisionEnter(gce::GameObject* other);
	void Death();


END_SCRIPT

