#include "ItemManager.h"
#include "ArenaManager.h"
#include "ItemCape.h"
#include "ItemApple.h"
#include "ItemBirthdayCakeSlice.h"
#include "ItemCerealBar.h"
#include "ItemCookie.h"
#include "ItemCoolWristband.h"
#include "ItemCrystalWings.h"
#include "ItemEnemyArmorPlate.h"
#include "ItemEnemyCoreGreen.h"
#include "ItemEnemyCoreOrange.h"
#include "ItemEnemyCoreRed.h"
#include "ItemEnemyJetpack.h"
#include "ItemKeyCard.h"
#include "ItemMagazine.h"
#include "ItemKey.h"
#include "ItemBlackFlask.h"
#include "GameUtils.h"

ItemManager* ItemManager::mp_instance = nullptr;

void ItemManager::Reset()
{
	m_itemMap.clear();

	for (auto drop : m_dropBase)
	{
		drop->GetGameObject()->Destroy();
	}

	m_dropBase.clear();
	for (auto ui : mp_itemUI)
	{
		ui.second->active = false;
		ui.second->mp_amountCounter->SetActive(false);
		ui.second->mp_frame->SetActive(false);
		ui.second->mp_itemImage->SetActive(false);
	}
}

void ItemManager::RegisterItem(ItemData* data)
{
	m_itemIDMap[data->itemID] = data;
	for (auto pool : data->m_itemPool)
	{
		switch (pool)
		{
		case ItemData::Pool::ENEMY:
			m_itemIDMapMob[data->itemID] = data;
			break;

		case ItemData::Pool::ROOM:
			m_itemIDMapFloor[data->itemID] = data;
			break;
		}
	}
}

void ItemManager::ActivateUi(bool active)
{
	mp_itemSideUI->SetActive(active);
	for (auto& ui : mp_itemUI)
	{
		if (ui.second->active == false)
		{
			continue;
		}
		ui.second->mp_frame->SetActive(active);
		ui.second->mp_itemImage->SetActive(active);
		ui.second->mp_amountCounter->SetActive(active);
	}

	if (active == false)
	{
		mp_itemName->SetActive(false);
		mp_itemDescription->SetActive(false);
		mp_itemInfoBackground->SetActive(false);
	}
}

void ItemManager::Awake()
{
	mp_player = ArenaManager::GetInstance()->GetPlayer();
	mp_itemName = gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()).AddComponent<gce::TextRenderer>();
	mp_itemName->rectPosF = new gce::RectanglePosF(215, 190, 715, 190);
	mp_itemName->scale = 0.8f;
	mp_itemName->text = L"";
	mp_itemName->pFont = ArenaManager::GetResourceManager()->GetFont(L"Jersey 10");
	mp_itemDescription = gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()).AddComponent<gce::TextRenderer>();
	mp_itemDescription->rectPosF = new gce::RectanglePosF(250, 170, 745, 270);
	mp_itemDescription->scale = 0.6f;
	mp_itemDescription->text = L"";
	mp_itemDescription->pFont = ArenaManager::GetResourceManager()->GetFont(L"Jersey 10");
	gce::Vector2f32 screenSize = { (float)gce::GameManager::GetWindowParam().width, (float)gce::GameManager::GetWindowParam().height };

	mp_itemInfoBackground = &CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/ItemInfo.png"), { screenSize.x * 0.4f, screenSize.y * 0.5f}, {4, 4}, {1, 0.5f});
	mp_itemSideUI = CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/boost.png"), screenSize * gce::Vector2f32(0.95f, 0.05f), {4, 4}).GetComponent<gce::UiImage>();

	RegisterItem(new ItemApple());
	RegisterItem(new ItemBirthdayCakeSlice());
	RegisterItem(new ItemCape());
	RegisterItem(new ItemCerealBar());
	RegisterItem(new ItemCookie());
	RegisterItem(new ItemCoolWristband());
	RegisterItem(new ItemCrystalWings());
	RegisterItem(new ItemEnemyArmorPlate());
	RegisterItem(new ItemEnemyCoreGreen());
	RegisterItem(new ItemEnemyCoreOrange());
	RegisterItem(new ItemEnemyCoreRed());
	RegisterItem(new ItemEnemyJetpack());
	RegisterItem(new ItemKey());
	RegisterItem(new ItemKeyCard());
	RegisterItem(new ItemMagazine());
	RegisterItem(new ItemBlackFlask());
}

void ItemManager::Start()
{
}

void ItemManager::Update()
{
	m_obtainable = false;
	float32 minDistance = INFINITE;
	for (ItemDropBase* drop : m_dropBase)
	{
		float32 distance = (mp_player->transform.GetWorldPosition() - drop->GetGameObject()->transform.GetWorldPosition()).Norm();
		if (minDistance > distance)
		{
			minDistance = distance;
			mp_closestDrop = drop;
		}
	}

	// Show Tooltip
	if (minDistance < m_obtainRange)
	{
		int width = gce::GameManager::GetWindow()->GetWidth();
		int height = gce::GameManager::GetWindow()->GetHeight();

		mp_itemName->rectPosF->top = 300.f;
		mp_itemDescription->rectPosF->top = 350.f;

		std::cout << "Object obtainable";
		m_obtainable = true;
		mp_itemName->text = mp_closestDrop->data->m_name;
		mp_itemDescription->text = mp_closestDrop->data->m_description;
		mp_itemName->SetActive(true);
		mp_itemDescription->SetActive(true);
		mp_itemInfoBackground->SetActive(true);
	}
	else
	{
		mp_itemName->SetActive(false);
		mp_itemDescription->SetActive(false);
		mp_itemInfoBackground->SetActive(false);
	}

	// Creation/Deletion step:
	for (auto add : m_itemToAdd)
	{
		m_itemMap[add] += 1;
		add->m_amount = m_itemMap[add];
		add->OnAdd();
	}

	m_itemToAdd.clear();
	for (auto remove : m_itemToRemove)
	{
		remove->OnRemove();

		m_itemMap.erase(remove);
	}
	m_itemToAdd.clear();
	m_itemToRemove.clear();
}

void ItemManager::PickItem(ItemDropBase* _drop)
{
	ItemData* item = _drop->data;
	bool exists = false;
	ItemData* existingItem = nullptr;
	for (std::map<ItemData*, int16>::iterator it = m_itemMap.begin(); it != m_itemMap.end(); ++it)
	{
		if (it->first->m_name == item->m_name)
		{
			existingItem = it->first;
			exists = true;
		}
	}

	if (!exists)
	{
		for (auto toAdd : m_itemToAdd)
		{
			if (toAdd->m_name == item->m_name)
			{
				existingItem = toAdd;
				exists = true;
			}
		}
	}

	if (exists)
	{
		m_itemMap[existingItem] += 1;
		existingItem->m_amount = m_itemMap[existingItem];
		ArenaManager::GetInstance()->GetPlayerData()->UpdateStats();
		existingItem->OnAdd();

		if(mp_itemUI[existingItem->itemID]->active)
		{
			mp_itemUI[existingItem->itemID]->mp_amountCounter->text = std::to_wstring(existingItem->m_amount);
			return;
		}
		CreateItemUI(item);
		return;
	}

	m_itemMap[item] = 1;
	item->m_amount = m_itemMap[item];
	ArenaManager::GetInstance()->GetPlayerData()->UpdateStats();
	item->OnAdd();

	// Add item UI
	CreateItemUI(item);
}

void ItemManager::ObtainItem()
{
	if (m_obtainable == false)
	{
		return;
	}

	PickItem(mp_closestDrop);

	m_dropBase.erase(std::find(m_dropBase.begin(), m_dropBase.end(), mp_closestDrop));
	delete mp_closestDrop;
}

void ItemManager::OnJump()
{
	for (std::map<ItemData*, int16>::iterator it = m_itemMap.begin(); it != m_itemMap.end(); ++it)
	{
		it->first->OnJump();
	}
}

void ItemManager::OnHit(gce::GameObject* other)
{
	for (std::map<ItemData*, int16>::iterator it = m_itemMap.begin(); it != m_itemMap.end(); ++it)
	{
		it->first->OnHit(other);
	}
}

void ItemManager::OnShoot(gce::GameObject* _bullet) {
	for (std::map<ItemData*, int16>::iterator it = m_itemMap.begin(); it != m_itemMap.end(); ++it)
	{
		it->first->OnShoot(_bullet);
	}
}

void ItemManager::OnSecondary()
{
	for (std::map<ItemData*, int16>::iterator it = m_itemMap.begin(); it != m_itemMap.end(); ++it)
	{
		it->first->OnSecondary();
	}
}

void ItemManager::CreateItemDropFor(ItemData* _data, gce::Vector3f32 _pos)
{
	ItemDropBase* itemDrop = new ItemDropBase();
	itemDrop->GetGameObject()->transform.SetWorldPosition(_pos);
	itemDrop->data = _data;
	itemDrop->InitGeometry();
	m_dropBase.push_back(itemDrop);
}

ItemData* ItemManager::GetRandomItem(ItemData::Pool pool)
{
	int itemAmount;
	int totalWeight;
	int select;
	int currentWeight;

	switch (pool)
	{
	case ItemData::Pool::ENEMY:
		itemAmount = m_itemIDMapMob.size();
		totalWeight = 0;

		for (auto idData : m_itemIDMapMob)
		{
			totalWeight += GetItemWeight(idData.second);
		}

		select = rand() % totalWeight;

		currentWeight = 0;
		for (auto idData : m_itemIDMapMob)
		{
			currentWeight += GetItemWeight(idData.second);
			if (select < currentWeight)
			{
				return idData.second;
			}
		}
		break;

	case ItemData::Pool::ROOM:
		itemAmount = m_itemIDMapFloor.size();
		totalWeight = 0;

		for (auto idData : m_itemIDMapFloor)
		{
			totalWeight += GetItemWeight(idData.second);
		}

		select = rand() % totalWeight;

		currentWeight = 0;
		for (auto idData : m_itemIDMapFloor)
		{
			currentWeight += GetItemWeight(idData.second);
			if (select < currentWeight)
			{
				return idData.second;
			}
		}
		break;
	}
}

int ItemManager::GetItemWeight(ItemData* data)
{
	switch (data->GetRarity())
	{
	case ItemData::COMMON:
		return 10;
	case ItemData::UNCOMMON:
		return 8;
	case ItemData::RARE:
		return 5;
	case ItemData::EPIC:
		return 3;
	case ItemData::LEGENDARY:
		return 1;
	}
}

bool ItemManager::HasItem(int id)
{
	for (auto data : m_itemMap)
	{
		if (data.first->GetID() == id)
		{
			return true;
		}
	}

	return false;
}

void ItemManager::CreateItemUI(ItemData* data)
{
 	int id = data->GetID();
	gce::Vector2f32 screenSize = { (float)gce::GameManager::GetWindowParam().width, (float)gce::GameManager::GetWindowParam().height };
	gce::Vector2f32 pos = (screenSize * gce::Vector2f32(0.95f, 0.05f));
	float x = (-17) * 4.f;
	int activeItemAmount = 0;
	for (auto ui : mp_itemUI)
	{
		if (ui.second->active == true)
		{
			activeItemAmount++;
		}
	}

	x *= activeItemAmount;
	x += (-58.f) * 2;

	gce::Vector2f32 offset = gce::Vector2f32(x, 0);
 	if (mp_itemUI.contains(id))
	{
		gce::BitMapBrush* frameBtm = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/boost_item.png");
		gce::Vector2f32 frameOriginOffset = gce::Vector2f32(0.5f, 0.5f) * frameBtm->GetSize() * gce::Vector2f32(4, 4);
		mp_itemUI[id]->mp_frame->InitializeImage(pos + offset - frameOriginOffset, frameBtm->GetSize() * gce::Vector2f32(4, 4), 1);
		mp_itemUI[id]->mp_frame->btmBrush = frameBtm;
		mp_itemUI[id]->mp_frame->btmBrush->SetTransformMatrix({ pos.x + offset.x - frameOriginOffset.x, pos.y + offset.y - frameOriginOffset.y, 0 }, { 4, 4, 1 }, 0);

		gce::BitMapBrush* itemBtm = ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/Items/" + std::to_string(id) + ".png");
		gce::Vector2f32 itemOriginOffset = gce::Vector2f32(0.5f, 0.5f) * itemBtm->GetSize() * gce::Vector2f32(0.15f, 0.15f);
		mp_itemUI[id]->mp_itemImage->InitializeImage(pos + offset - itemOriginOffset, itemBtm->GetSize() * gce::Vector2f32( 0.15f, 0.15f ), 1);
		mp_itemUI[id]->mp_itemImage->btmBrush = itemBtm;
		mp_itemUI[id]->mp_itemImage->btmBrush->SetTransformMatrix({ pos.x + offset.x - itemOriginOffset.x, pos.y + offset.y - itemOriginOffset.y, 0 }, { 0.15f, 0.15f, 1 }, 0);

		mp_itemUI[id]->mp_amountCounter->text = std::to_wstring(data->m_amount);
		mp_itemUI[id]->mp_amountCounter->scale = 0.5f;

		if (mp_itemUI[id]->mp_amountCounter->rectPosF != nullptr)
		{
			delete mp_itemUI[id]->mp_amountCounter->rectPosF;
		}

		mp_itemUI[id]->mp_amountCounter->rectPosF = new gce::RectanglePosF(mp_itemUI[id]->mp_frame->position.x, 100, 50, 0);

		mp_itemUI[id]->active = true;
		mp_itemUI[id]->mp_frame->SetActive(true);
		mp_itemUI[id]->mp_itemImage->SetActive(true);
		mp_itemUI[id]->mp_amountCounter->SetActive(true);
	}

	else
	{
		gce::GameObject& imageObject = CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/boost_item.png"), pos + offset, { 4, 4 });
		gce::GameObject& itemImageObject = CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/Items/" + std::to_string(data->GetID()) + ".png"), pos + offset, { 0.15f, 0.15f });

		ItemUI* itemUI = new ItemUI();
		itemUI->mp_frame = imageObject.GetComponent<gce::UiImage>();
		itemUI->mp_itemImage = itemImageObject.GetComponent<gce::UiImage>();
		itemUI->mp_amountCounter = itemImageObject.AddComponent<gce::TextRenderer>();
		itemUI->mp_amountCounter->text = std::to_wstring(data->m_amount);
		itemUI->mp_amountCounter->scale = 0.5f;
		itemUI->mp_amountCounter->rectPosF = new gce::RectanglePosF(itemUI->mp_frame->position.x, 50, 100, 0);
		itemUI->active = true;

		mp_itemUI[data->itemID] = itemUI;
	}
	
}
