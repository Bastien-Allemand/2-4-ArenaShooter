#include "ItemEnemyJetpack.h"

ItemEnemyJetpack::ItemEnemyJetpack() {
	itemID = 11;
	m_rarity = Rarity::UNCOMMON;
	m_itemPool.push_back(Pool::ENEMY);
	m_name = L"Enemy Jetpack";
	m_description = L"Increases movement speed";
	m_itemStats.moveSpeed = 1;
}