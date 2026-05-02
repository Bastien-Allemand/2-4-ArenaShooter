#include "Zone.h"
#include "ArenaManager.h"
#include "MeleeEnemy.h"
#include "LaserEnemy.h"
#include "HealEnemy.h"

void ZoneScript::CollisionEnter(gce::GameObject* other)
{
	if (m_tookAggro == true)
	{
		return;
	}

	if (other->HasScript<Player>())
	{
		m_tookAggro = true;
		for (auto enemy : EnemyInZone)
		{
			BotEnemy* script = enemy->GetScript<MeleeEnemy>();
			script = script == nullptr ? enemy->GetScript<LaserEnemy>() : script;
			script = script == nullptr ? enemy->GetScript<HealEnemy>() : script;

			if (script != nullptr)
			{
				script->Aggro();
			}
		}
	}

}

void ZoneScript::Reset()
{
	m_tookAggro = false;
	m_cleared = false;
	m_inactiveAmount = 0;
}

void ZoneScript::Update()
{
	
}

void ZoneScript::EnemyDeath()
{
	if (m_cleared)
	{
		return;
	}

	m_inactiveAmount++;

	if (m_inactiveAmount == EnemyInZone.Size())
	{
		ArenaManager::GetItemManager()->CreateItemDropFor
		(
			ArenaManager::GetItemManager()->GetRandomItem(ItemData::ROOM),
			m_pOwner->transform.GetWorldPosition()
		);
		m_cleared = true;
	}
}

ObjectReach* ZoneScript::GetClosestEnemy(gce::Vector3f32 position, gce::GameObject* _exception)
{
	ObjectReach* reach = new ObjectReach();
	reach->pgameObject = nullptr;
	reach->obstructed = false;
	float minimumDelta = INFINITE;
	gce::Vector3f32 deltaVect;
	for (auto enemy : EnemyInZone) {
		if (enemy == _exception || enemy->IsActive() == false)
		{
			continue;
		}
		deltaVect = enemy->transform.GetWorldPosition() - position;
		if (deltaVect.Norm() < minimumDelta) {
			minimumDelta = deltaVect.Norm();
			reach->pgameObject = enemy;
		}
	}
	gce::Ray ray = { position, deltaVect.Normalize() };
	gce::RaycastHit info;
	info.pGameObject = nullptr;
	gce::GameManager::GetPhysicSystem().IntersectRay(ray, info, minimumDelta);
	if (info.pGameObject == nullptr)
	{
		reach->obstructed = false;
	}
	else if (info.pGameObject->GetName() == "Wall")
	{
		// Blocked
		std::cout << "Vision blocked";
		reach->obstructed = true;
	}


	return reach;
}
