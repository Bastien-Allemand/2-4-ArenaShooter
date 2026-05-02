#include "ItemKey.h"

ItemKey::ItemKey()
{
	m_rarity = Rarity::LEGENDARY;
	m_itemPool.push_back(Pool::PROP);
	itemID = 100;
	m_name = L"Lab Keys";
	m_description = L"Gives access to the next room.";
}
