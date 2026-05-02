#include "ItemApple.h"
#include "ArenaManager.h"

ItemApple::ItemApple() {
	itemID = 1;
	m_rarity = Rarity::COMMON;
	m_itemPool.push_back(Pool::PROP);
	m_name = L"Apple";
	m_description = L"Increases maximum Health by 10";
	m_itemStats.maxHealth = 10;
}

void ItemApple::OnAdd()
{
	ArenaManager::GetInstance()->GetPlayerData()->currentHealth += m_itemStats.maxHealth;
}