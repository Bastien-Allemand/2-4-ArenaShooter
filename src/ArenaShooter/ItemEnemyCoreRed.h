#pragma once
#include "Item.h"

class ItemEnemyCoreRed : public ItemData
{
public:
	ItemEnemyCoreRed();
	void OnHit(gce::GameObject* other);
};

DECLARE_SCRIPT(Bleed, ScriptFlag::Update)

int m_stackAmount = 1;
float lifeTime = 3;
float damageCountdown = 0.2;

void Update();

END_SCRIPT