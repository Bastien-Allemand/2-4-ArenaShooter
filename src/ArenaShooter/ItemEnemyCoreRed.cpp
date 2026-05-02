#include "ItemEnemyCoreRed.h"
#include "ArenaManager.h"
#include "Crate.h"
#include "Enemy.h"

ItemEnemyCoreRed::ItemEnemyCoreRed() {
	itemID = 10;
	m_rarity = Rarity::RARE;
	m_itemPool.push_back(Pool::ENEMY);
	m_name = L"Red Enemy Core";
	m_description = L"Gives a chance to inflict Bleed to enemies on hit";
}

void ItemEnemyCoreRed::OnHit(gce::GameObject* other) {
	if (not other->HasScript<Crate>() && not other->HasScript<Enemy>()) {
		return;
	}
	int burnChance = (rand() % 100) + 1;
	if (burnChance <= (m_amount * 10)) {
		if (other->GetScript<Bleed>() != nullptr) {
			other->GetScript<Bleed>()->m_stackAmount += 1;
			other->GetScript<Bleed>()->lifeTime = 3;
		}
		else {
			other->AddScript<Bleed>();
		}
	}
}


void Bleed::Update() {
	float deltatime = gce::GameManager::DeltaTime();

	lifeTime -= deltatime;
	damageCountdown -= deltatime;

	if (damageCountdown <= 0) {
		if (m_pOwner->HasScript<Crate>()) {
			m_pOwner->GetScript<Crate>()->TakeDamage((ArenaManager::GetInstance()->GetPlayerData()->damage / 8) * m_stackAmount);
		}
		else if (m_pOwner->HasScript<GameActor>()) {
			m_pOwner->GetScript<GameActor>()->TakeDamage((ArenaManager::GetInstance()->GetPlayerData()->damage / 8) * m_stackAmount);
		}
		damageCountdown = 0.2;
	}
	if (lifeTime <= 0) {
		m_pOwner->RemoveScript<Bleed>();
	}
}