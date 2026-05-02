#include "Bullet.h"
#include "Elevator.h"
#include "Zone.h"
#include "LaserBullet.h"

void Bullet::Start() {
	m_forward = m_pOwner->transform.GetLocalForward();
}

void Bullet::Update() {
	float deltatime = gce::GameManager::DeltaTime();
	//m_forward = m_pOwner->transform.GetLocalForward();

	bulletLifetime += deltatime;
	if (bulletLifetime >= bulletMaxLifetime) {
		m_pOwner->Destroy();
		std::cout << "bulet gone" << std::endl;
	}
}

void Bullet::FixedUpdate() {
	m_pOwner->transform.LocalTranslate(m_forward * (bulletSpeed) * gce::GameManager::FixedDeltaTime());
}

void Bullet:: CollisionEnter(gce::GameObject* other) {
	if (isDestroyed || other->HasScript<Player>() || other->GetName() == "Elevator" || other->GetName() == "Zone" || other->HasScript<LaserBullet>()) { return; }

	ArenaManager::GetItemManager()->OnHit(other);

	isDestroyed = true;
	std::cout << "Bullet Collision" << std::endl;
	m_pOwner->Destroy();
}