#pragma once
#include "Enemy.h"

class BossForcefield;

struct BossHealthbar
{
	gce::GameObject* background;
	gce::GameObject* top;
	gce::GameObject* bar;
};

DECLARE_CHILD_SCRIPT(Boss, Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::Destroy)

enum Pattern
{
	BULLET,
	LASER,
	MISSILE,
	DASH
};

// DASH
bool m_firstBarrierActivated = false;
bool m_secondBarrierActivated = false;
bool m_invulnerable = false;

float m_dashChrono = 0.f;
float m_dashTime;
float m_telegraphChrono = 0.f;
float m_telegraphTime;

gce::Vector3f32 m_dashDirection;
float m_dashSpeed;
gce::GameObject* mp_forcefield;
BossForcefield* mp_dashHitboxScript;
float m_dashDamage = 75.f;

// IDLE
float m_idleChrono = 0.f;
float m_idleTime;

// Shoot
float m_shotChrono = 0.f;
float m_bulletShootingChrono = 0.f;
float m_shotTime;
float m_bulletShootingTime;

// Missile
float m_missileChrono = 0.f;
float m_missileShootingChrono = 0.f;
float m_missileTime;
float m_missileShootingTime;
int m_missilesShot;
int m_missileAmount;

// visuals
gce::Vector3f32 m_bulletOffset;
gce::Vector3f32 m_missileOffset;

BossHealthbar m_healthbar;

void ChangePattern();

void Start();
void Update();
void Destroy();


END_SCRIPT

class LaserBullet;

DECLARE_SCRIPT(MissileHoming, ScriptFlag::Start | ScriptFlag::Update)
float homingStrength;
gce::GameObject* player;
LaserBullet* script;

void Start();
void Update();

END_SCRIPT

DECLARE_SCRIPT(BossForcefield, ScriptFlag::CollisionEnter)

Boss* mp_bossScript;
bool hit = false;
bool damaging = false;

void CollisionEnter(gce::GameObject* other);


END_SCRIPT