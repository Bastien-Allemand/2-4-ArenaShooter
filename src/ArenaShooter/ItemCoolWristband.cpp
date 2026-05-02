#include "ItemCoolWristband.h"
#include "ArenaManager.h"
#include "Bullet.h"

ItemCoolWristband::ItemCoolWristband() {
	itemID = 5;
	m_rarity = Rarity::EPIC;
	m_itemPool.push_back(Pool::PROP);
	m_itemPool.push_back(Pool::ROOM);
	m_name = L"Cool Wristband";
	m_description = L"Gives your bullets a homing effect";
}

void ItemCoolWristband::OnShoot(gce::GameObject* _bullet) {
	_bullet->AddScript<Homing>()->itemAmount = m_amount;
}

void Homing::Start() {
	homingStrength = std::logf(itemAmount + 1);
	range = 10.f + itemAmount * 2.f;
}

void Homing::Update() {
	gce::GameObject* target = ArenaManager::GetInstance()->GetClosestEnemy(m_pOwner->transform.GetWorldPosition());
	Bullet* script = m_pOwner->GetScript<Bullet>();
	if (target == nullptr || script == nullptr) {
		return;
	}

	gce::Vector3f32 delta = (target->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition());

	if (delta.Norm() >= range)
	{
		return;
	}

	delta = delta.Normalize();

	gce::Vector3f32 direction = gce::Vector3f32::Lerp(script->m_forward, delta, homingStrength * gce::GameManager::DeltaTime()).Normalize();
	script->m_forward = direction;
}