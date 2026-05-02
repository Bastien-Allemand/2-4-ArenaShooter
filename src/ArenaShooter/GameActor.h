#pragma once
#include "Engine.h"

DECLARE_SCRIPT(GameActor, ScriptFlag::Update)

float maxHealth;
float currentHealth;
int armor;

float shotCooldown;
float currentShotCooldown;
int damage;

float moveSpeed;

void Heal(float _value);
virtual void TakeDamage(float damageValue);
virtual void Death() {};

virtual void Update() {};

END_SCRIPT