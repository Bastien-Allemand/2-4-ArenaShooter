#pragma once
#include "define.h"
#include <Engine.h>
#include "Player.h"


class ItemData
{
public:
	enum Rarity
	{
		COMMON,
		UNCOMMON,
		RARE,
		EPIC,
		LEGENDARY
	};

	enum Pool
	{
		PROP,
		ROOM,
		ENEMY
	};

protected:
	int itemID;

	WStringView m_name;
	WStringView m_description;
	Rarity m_rarity;
	std::vector<Pool> m_itemPool;
	int m_amount;
	PlayerStats m_itemStats;

	virtual void OnAdd() {};
	virtual void OnRemove() {};
	virtual void OnShoot(gce::GameObject* _bullet) {};
	virtual void OnHit(gce::GameObject* _other) {}; // TODO: add target in arguments
	virtual void OnJump() {};
	virtual void OnSecondary() {};

public:
	int GetID() { return itemID; };
	PlayerStats& GetStats() { return m_itemStats; };
	Rarity GetRarity() { return m_rarity; };

	friend class ItemManager;
};

class ItemDebug: public ItemData
{
public:
	ItemDebug() 
	{ 
		m_name = L"Debug"; 
		m_description = L"Debug description"; 
	
	};

private:
	void OnAdd()
	{
		std::cout << "Item Debug Added: " << m_amount << std::endl;
	}

	void OnJump()
	{
		for (int16 i = 0; i < m_amount; i++)
		{
			std::cout << "Item Debug procs: " << i << std::endl;
		}
	}

	friend class ItemManager;
};