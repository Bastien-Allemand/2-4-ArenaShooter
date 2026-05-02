#include "ItemCerealBar.h"

ItemCerealBar::ItemCerealBar() {
	itemID = 4;
	m_rarity = Rarity::UNCOMMON;
	m_itemPool.push_back(Pool::PROP);
	m_name = L"Cereal bar";
	m_description = L"Increases Health regeneration by 0.5 per second";
	m_itemStats.healthRegen = 0.5;
}