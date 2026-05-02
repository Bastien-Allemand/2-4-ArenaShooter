#pragma once
#include "GameActor.h"
#include <Engine.h>

struct PlayerStats
{
	// Defensive
	float maxHealth = 0;
	float healthRegen = 0;
	int armor = 0;
	// Offensive
	float atkSpeed = 0;
	int damage = 0;
	int ammoCapacity = 0;
	// Movement
	float moveSpeed = 0;
	float runSpeedValue = 0;
	float crouchSlowValue = 0;

	PlayerStats& operator+=(PlayerStats const& other)
	{
		maxHealth += other.maxHealth;
		healthRegen += other.healthRegen;
		armor += other.armor;
		atkSpeed += other.atkSpeed;
		damage += other.damage;
		ammoCapacity += other.ammoCapacity;
		moveSpeed += other.moveSpeed;
		runSpeedValue += other.runSpeedValue;
		crouchSlowValue += other.crouchSlowValue;

		return *this;
	}
};

DECLARE_CHILD_SCRIPT(Player, GameActor, ScriptFlag::Awake | ScriptFlag::Start | ScriptFlag::Update)
PlayerStats m_baseStats;

const float regenCooldown = 1.0;
float currentRegenCooldown = regenCooldown;
float healthRegen;

int ammoCapacity;
int ammoLeft;

float atkSpeed;
 
float runSpeedValue;
float crouchSlowValue;

const float damageFeedbackTimer = 0.05f;
float currentDamageFeedbackTimer = 0;
bool hasDamaged = false;

// ---- Player UI ----
//gce::UiImage* 
gce::GameObject* mp_crosshair;
gce::GameObject* mp_ammoIcon;
gce::GameObject* mp_healthIcon;
gce::TextRenderer* mp_healthDisplay;
gce::TextRenderer* mp_ammoDisplay;

void Reset();

void ActivateUI(bool active);

void Awake();
void Start();
virtual void Death() override;
virtual void Update() override;
virtual void TakeDamage(float damageValue) override;

void SetStats(PlayerStats const& _stats);

public:
void UpdateStats();

END_SCRIPT
