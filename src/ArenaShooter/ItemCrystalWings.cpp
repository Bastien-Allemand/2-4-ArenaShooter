#include "ItemCrystalWings.h"

ItemCrystalWings::ItemCrystalWings() {
	itemID = 6;
	m_rarity = Rarity::LEGENDARY;
	m_itemPool.push_back(Pool::ROOM);
	m_name = L"Crystal Wings";
	m_description = L"Increases Health regen, drastically increases Speed, but decreases Armor";
	m_itemStats.moveSpeed = 3;
	m_itemStats.armor = -10;
	m_itemStats.healthRegen = 1;
}