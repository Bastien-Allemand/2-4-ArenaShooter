#pragma once
#include "Item.h"

class ItemEnemyCoreGreen : public ItemData
{
public:
	ItemEnemyCoreGreen();
	void OnHit(gce::GameObject* other);
};

DECLARE_SCRIPT (Poison, ScriptFlag::Update)

int m_stackAmount = 1;
float lifeTime = 3;
float damageCountdown = 0.5;

void Update();

END_SCRIPT