#include "Crate.h"
#include "ArenaManager.h"
#include "Bullet.h"

void Crate::setItemId(int idNumber) {
	itemId = idNumber;
}

void Crate::CollisionEnter(gce::GameObject* other) {
	if (other->HasScript<Bullet>()) {
		TakeDamage(ArenaManager::GetInstance()->GetPlayerData()->damage);
	}
}

void Crate::TakeDamage(int damageValue) {
	float damageTaken;
	if (armor > -99) {
		damageTaken = damageValue / (1.0f + armor / 100.0f);
	}
	else {
		damageTaken = damageValue / (1.0f - 99.0f / 100.0f);
	}
	currentHealth -= damageTaken;

	if (currentHealth <= 0) {
		currentHealth = 0;
		Death();
	}
}

void Crate::Death() {
	std::cout << "crate deth" << std::endl;
	m_pOwner->SetActive(false);
	//Drop the item
	if (itemId == 0)
	{
		return;
	}

	ItemManager* itemManager = ArenaManager::GetInstance()->GetItemManager();
	itemManager->CreateItemDropFor(itemManager->m_itemIDMap[itemId], m_pOwner->transform.GetWorldPosition());
}