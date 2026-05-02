#pragma once
#include "Item.h"
#include "ItemDropBase.h"
#include <Engine.h>

struct ItemUI
{
	bool active;

	gce::UiImage* mp_frame;
	gce::UiImage* mp_itemImage;
	gce::TextRenderer* mp_amountCounter;
	std::wstring m_amountString;
};

DECLARE_SCRIPT(ItemManager, ScriptFlag::Awake | ScriptFlag::Start | ScriptFlag::Update)

std::map<ItemData*, int16> m_itemMap;
std::map<int, ItemData*> m_itemIDMap;
std::map<int, ItemData*> m_itemIDMapFloor;
std::map<int, ItemData*> m_itemIDMapMob;
std::vector<ItemDropBase*> m_dropBase;
static ItemManager* mp_instance;
gce::GameObject* mp_player;
ItemDropBase* mp_closestDrop;
bool m_obtainable;
const float32 m_obtainRange = 5.f;
gce::TextRenderer* mp_itemName;
gce::TextRenderer* mp_itemDescription;
gce::GameObject* mp_itemInfoBackground;
std::vector<ItemData*> m_itemToAdd;
std::vector<ItemData*> m_itemToRemove;

gce::UiImage* mp_itemSideUI;
std::unordered_map<int, ItemUI*> mp_itemUI;

public:
	void Reset();

	template <typename t>
	void AddItem();

	template <typename t>
	void RemoveItem();

	void PickItem(ItemDropBase* _drop);

	template <typename t>
	void CreateItemDrop(gce::Vector3f32 _pos);

	void CreateItemDropFor(ItemData* , gce::Vector3f32 _pos);

	void ObtainItem();
	bool HasItem(int id);

	void CreateItemUI(ItemData* data);

	void ActivateUi(bool active);

	ItemData* GetRandomItem(ItemData::Pool pool);

	int GetItemWeight(ItemData* data);

	// Add every possible player event.
	void OnShoot(gce::GameObject* _bullet);
	void OnHit(gce::GameObject* _other); // TODO: add target in arguments
	void OnJump();
	void OnSecondary();

	void Awake();
	void Start();
	void Update();

	void RegisterItem(ItemData* data);


END_SCRIPT

#include "ItemManager.inl"