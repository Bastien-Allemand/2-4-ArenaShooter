#pragma once
#include "Engine.h"

DECLARE_SCRIPT(Crate, ScriptFlag::CollisionEnter)

int maxHealth = 30;
int currentHealth = maxHealth;
int armor = 10;

int itemId = 0;

void setItemId(int idNumber);

void CollisionEnter(gce::GameObject* other);

void TakeDamage(int damageValue);
void Death();

END_SCRIPT
