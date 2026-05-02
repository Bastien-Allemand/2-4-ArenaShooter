#include "ItemBlackFlask.h"
#include "ArenaManager.h"

ItemBlackFlask::ItemBlackFlask() {
	itemID = 15;
	m_rarity = Rarity::LEGENDARY;
	m_itemPool.push_back(Pool::PROP);
	m_itemPool.push_back(Pool::ROOM);
	m_name = L"Black Flask";
	m_description = L"Increases damage and decreases Armor by 1000, you are nearsighted";
	m_itemStats.damage = 1000;
	m_itemStats.armor = -1000;
}

void ItemBlackFlask::OnAdd() {
	ArenaManager::GetInstance()->GetCamera()->perspective.farPlane = 30.0f;
}