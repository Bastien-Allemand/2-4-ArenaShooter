#include "LaserBullet.h"

void LaserBullet::Start() {

}

void LaserBullet::Update() {
	float deltatime = gce::GameManager::DeltaTime();

	bulletLifetime += deltatime;
	if (bulletLifetime >= bulletMaxLifetime) {
		m_pOwner->Destroy();
	}
}

void LaserBullet::FixedUpdate() {
	m_pOwner->transform.LocalTranslate(m_forward * bulletSpeed * gce::GameManager::FixedDeltaTime());
}

void LaserBullet::CollisionEnter(gce::GameObject* other) {
	if (isDestroyed) { return; }
	if (other->HasScript<Player>() == false && other->GetName() != "Wall" && other->GetName() != "Crate")
	{
		return;
	}

	Player* playerScript = other->GetScript<Player>();
	if (playerScript != nullptr)
	{
		playerScript->TakeDamage(m_damage);
	}

	isDestroyed = true;
	m_pOwner->Destroy();
}