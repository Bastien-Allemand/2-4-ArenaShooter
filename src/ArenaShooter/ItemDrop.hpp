#pragma once
#include "define.h"
#include <Engine.h>
#include "ItemDropBase.h"

template <typename t>
class ItemDrop : public ItemDropBase
{
public:
	ItemDrop();
};

#include "ItemDrop.inl"