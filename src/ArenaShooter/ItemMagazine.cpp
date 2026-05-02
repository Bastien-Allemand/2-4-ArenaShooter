#include "ItemMagazine.h"

ItemMagazine::ItemMagazine() {
	itemID = 14;
	m_rarity = Rarity::COMMON;
	m_itemPool.push_back(Pool::ENEMY);
	m_itemPool.push_back(Pool::PROP);
	m_name = L"Magazine";
	m_description = L"Increases ammo capacity by 3";
	m_itemStats.ammoCapacity = 3;
}