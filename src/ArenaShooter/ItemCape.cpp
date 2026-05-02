#include "ItemCape.h"
#include "ArenaManager.h"


ItemCape::ItemCape()
{
	m_rarity = Rarity::RARE;
	m_itemPool.push_back(Pool::PROP);
	itemID = 13;
	m_name = L"Cool Cape";
	m_description = L"Increases movement speed significantly";
	m_itemStats.moveSpeed = 3;
}
