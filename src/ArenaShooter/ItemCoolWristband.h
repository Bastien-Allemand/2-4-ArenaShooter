#pragma once
#include "Item.h"

class ItemCoolWristband : public ItemData
{
public:
	ItemCoolWristband();
	void OnShoot(gce::GameObject* _bullet);
};

DECLARE_SCRIPT (Homing, ScriptFlag::Start | ScriptFlag::Update)
int itemAmount;
float homingStrength;
float range;

void Start();
void Update();

END_SCRIPT
