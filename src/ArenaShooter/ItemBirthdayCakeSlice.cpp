#include "ItemBirthdayCakeSlice.h"
#include "ArenaManager.h"

ItemBirthdayCakeSlice::ItemBirthdayCakeSlice() {
	itemID = 3;
	m_rarity = Rarity::RARE;
	m_itemPool.push_back(Pool::PROP);
	m_name = L"Birthday cake slice";
	m_description = L"Increases maximum Health by 50";
	m_itemStats.maxHealth = 50;
}

void ItemBirthdayCakeSlice::OnAdd() {
	ArenaManager::GetInstance()->GetPlayerData()->currentHealth += m_itemStats.maxHealth;
}