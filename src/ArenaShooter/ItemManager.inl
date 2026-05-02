#include "ItemManager.h"
#include "ItemDrop.hpp"
#include "GameUtils.h"

template<typename t>
inline void ItemManager::AddItem()
{
	t* item = new t();
	bool exists = false;
	t* existingItem = nullptr;
	for (std::map<ItemData*, int16>::iterator it = m_itemMap.begin(); it != m_itemMap.end(); ++it)
	{
		if (it->first->m_name == item->m_name)
		{
			existingItem = (t*)it->first;
			exists = true;
		}
	}

	if (!exists)
	{
		for (auto toAdd : m_itemToAdd)
		{
			if (toAdd->m_name == item->m_name)
			{
				existingItem = (t*)toAdd;
				exists = true;
			}
		}
	}

	if (exists)
	{
		m_itemMap[existingItem] += 1;
		existingItem->m_amount = m_itemMap[existingItem];
		existingItem->OnAdd();
		return;
	}
	m_itemToAdd.push_back(item);

	gce::Vector2f32 screenSize = { (float)gce::GameManager::GetWindowParam().width, (float)gce::GameManager::GetWindowParam().height };
	gce::Vector2f32 center = screenSize / 2.f;
	CreateImage(new gce::BitMapBrush("res/UI/boost_item.png"), screenSize * gce::Vector2f32(0.95f, 0.05f) - gce::Vector2f32(18, 0), { 2, 2 });
}

template<typename t>
inline void ItemManager::RemoveItem()
{
	t* item = new t();
	bool exists = false;
	for (std::map<ItemData*, int16>::iterator it = m_itemMap.begin(); it != m_itemMap.end(); ++it)
	{
		if (it->first->m_name == item->m_name)
		{
			m_itemToRemove.push_back(it->first);
			//m_itemMap.erase(it);
			return;
		}
	}

	return;
}


template<typename t>
inline void ItemManager::CreateItemDrop(gce::Vector3f32 _pos)
{
	ItemDrop<t>* itemDrop = new ItemDrop<t>();
	itemDrop->GetGameObject()->transform.SetWorldPosition(_pos);
	itemDrop->data = new t();
	itemDrop->InitGeometry();
	m_dropBase.push_back(itemDrop);
}
