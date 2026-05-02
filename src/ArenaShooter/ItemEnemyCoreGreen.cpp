#include "ItemEnemyCoreGreen.h"
#include "ArenaManager.h"
#include "Crate.h"
#include "Enemy.h"

ItemEnemyCoreGreen::ItemEnemyCoreGreen() {
	itemID = 8;
	m_rarity = Rarity::RARE;
	m_itemPool.push_back(Pool::ENEMY);
	m_name = L"Green Enemy Core";
	m_description = L"Gives a chance to inflict Poison to enemies on hit";
}

void ItemEnemyCoreGreen::OnHit(gce::GameObject* other) {
	if (not other->HasScript<Crate>() && not other->HasScript<Enemy>()) {
		return;
	}
	int poisonChance = (rand() % 100) + 1;
	if (poisonChance <= (m_amount * 10)) {
		if (other->GetScript<Poison>() != nullptr) {
			other->GetScript<Poison>()->m_stackAmount += 1;
			other->GetScript<Poison>()->lifeTime = 3;
		}
		else {
			other->AddScript<Poison>();
		}
	}
}


void Poison::Update() {
	float deltatime = gce::GameManager::DeltaTime();

	lifeTime -= deltatime;
	damageCountdown -= deltatime;

	if (damageCountdown <= 0) {
		if (m_pOwner->HasScript<Crate>()) {
			m_pOwner->GetScript<Crate>()->TakeDamage((ArenaManager::GetInstance()->GetPlayerData()->damage / 5)*m_stackAmount);
		}
		else if (m_pOwner->HasScript<GameActor>()) {
			m_pOwner->GetScript<GameActor>()->TakeDamage((ArenaManager::GetInstance()->GetPlayerData()->damage / 5) * m_stackAmount);
		}
		damageCountdown = 0.5;
	}
	if (lifeTime <= 0) {
		m_pOwner->RemoveScript<Poison>();
	}
}