#pragma once
#include <Engine.h>
#include "define.h"
#include "ArenaManager.h"

DECLARE_SCRIPT(Bullet, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::FixedUpdate | ScriptFlag::CollisionEnter)

gce::Vector3f32 m_forward;

float bulletSpeed = 65.f;
float bulletLifetime = 0.0;
float bulletMaxLifetime = 3.0;

bool isDestroyed = false;

void Start();
void Update();
void FixedUpdate();
void CollisionEnter(gce::GameObject* other);

END_SCRIPT
