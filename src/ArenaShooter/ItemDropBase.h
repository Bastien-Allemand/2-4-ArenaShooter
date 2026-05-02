#pragma once
#include "define.h"
#include "Item.h"
#include <Engine.h>

class ItemDropBase
{
protected:
	gce::GameObject* mp_itemSphere;

public:
	ItemData* data;

	ItemDropBase();
	void InitGeometry();
	~ItemDropBase();
	gce::GameObject* GetGameObject() { return mp_itemSphere; };
};