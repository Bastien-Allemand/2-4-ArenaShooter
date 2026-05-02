#pragma once
#include "Item.h"

class ItemEnemyCoreOrange : public ItemData
{
public:
	ItemEnemyCoreOrange();
	void OnHit(gce::GameObject* other);
};

DECLARE_SCRIPT(Burn, ScriptFlag::Update)

	int m_stackAmount = 1;
	float lifeTime = 5;
	float damageCountdown = 1;

	void Update();

END_SCRIPT
