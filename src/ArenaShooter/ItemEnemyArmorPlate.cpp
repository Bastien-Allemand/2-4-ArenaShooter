#include "ItemEnemyArmorPlate.h"

ItemEnemyArmorPlate::ItemEnemyArmorPlate() {
	itemID = 7;
	m_rarity = Rarity::COMMON;
	m_itemPool.push_back(Pool::ENEMY);
	m_name = L"Enemy armor plate";
	m_description = L"Slightly increases Armor";
	m_itemStats.armor = 5;
}