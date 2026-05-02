#include "ItemCookie.h"
#include "ArenaManager.h"

ItemCookie::ItemCookie() {
	itemID = 2;
	m_rarity = Rarity::UNCOMMON;
	m_itemPool.push_back(Pool::PROP);
	m_name = L"Cookie";
	m_description = L"Increases maximum Health by 25";
	m_itemStats.maxHealth = 25;
}

void ItemCookie::OnAdd() {
	ArenaManager::GetInstance()->GetPlayerData()->currentHealth += m_itemStats.maxHealth;
}