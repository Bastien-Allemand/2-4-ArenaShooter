#include "ItemKeyCard.h"
#include "ArenaManager.h"

ItemKeyCard::ItemKeyCard() {
	itemID = 12;
	m_rarity = Rarity::RARE;
	m_itemPool.push_back(Pool::ROOM);
	m_itemPool.push_back(Pool::ENEMY);
	m_name = L"Employee Key Card";
	m_description = L"Increases Speed & Damage slightly, and Attack Speed moderately";
	m_itemStats.moveSpeed = 0.5;
	m_itemStats.atkSpeed = 15;
	m_itemStats.damage = 5;
}