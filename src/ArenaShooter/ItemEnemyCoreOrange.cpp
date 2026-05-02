#include "ItemEnemyCoreOrange.h"
#include "ArenaManager.h"
#include "Crate.h"
#include "Enemy.h"

ItemEnemyCoreOrange::ItemEnemyCoreOrange() {
	itemID = 9;
	m_rarity = Rarity::RARE;
	m_itemPool.push_back(Pool::ENEMY);
	m_name = L"Orange Enemy Core";
	m_description = L"Gives a chance to inflict Burn to enemies on hit";
}

void ItemEnemyCoreOrange::OnHit(gce::GameObject* other) {
	if (not other->HasScript<Crate>() && not other->HasScript<Enemy>()) {
		return;
	}
	int burnChance = (rand() % 100) + 1;
	if (burnChance <= (m_amount * 10)) {
		if (other->GetScript<Burn>() != nullptr) {
			other->GetScript<Burn>()->m_stackAmount += 1;
			other->GetScript<Burn>()->lifeTime = 5;
		}
		else {
			other->AddScript<Burn>();
		}
	}
}


void Burn::Update() {
	float deltatime = gce::GameManager::DeltaTime();

	lifeTime -= deltatime;
	damageCountdown -= deltatime;

	if (damageCountdown <= 0) {
		if (m_pOwner->HasScript<Crate>()) {
			m_pOwner->GetScript<Crate>()->TakeDamage((ArenaManager::GetInstance()->GetPlayerData()->damage / 2) * m_stackAmount);
		}
		else if (m_pOwner->HasScript<GameActor>()) {
			m_pOwner->GetScript<GameActor>()->TakeDamage((ArenaManager::GetInstance()->GetPlayerData()->damage / 2) * m_stackAmount);
		}
		damageCountdown = 1;
	}
	if (lifeTime <= 0) {
		m_pOwner->RemoveScript<Burn>();
	}
}