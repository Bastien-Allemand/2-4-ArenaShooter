#include "BotEnemy.h"
#include <Engine/StateMachine.h>
#include "ArenaManager.h"
#include "Maploader.h"
#include "Zone.h"
#include "Bullet.h"
#include "Pathfinding.hpp"

void BotEnemy::Start()
{
	Enemy::Start();

	states->AddAction("Idle", nullptr, nullptr, nullptr);
	states->AddAction("Telegraph", nullptr, nullptr, nullptr);
	states->AddAction("Attack", nullptr, nullptr, nullptr);
	states->AddAction("Roam", nullptr, nullptr, nullptr);
	states->AddAction("GetInRange", nullptr, nullptr, nullptr);
	states->Transit("Idle");
}

void BotEnemy::Reset()
{
	m_pOwner->GetComponent<gce::BoxCollider>()->SetActive(true);

	states->Transit("Idle");
	roomRegistered = false;
	currentHealth = maxHealth;
	isDying = false;
}

void BotEnemy::CollisionEnter(gce::GameObject* other)
{
	if (roomRegistered == false && other->GetName() == "Zone")
	{
		pZone = other->GetScript<ZoneScript>();
		pZone->EnemyInZone.PushBack(m_pOwner);
	}

	if (other->HasScript<Bullet>()) {
		TakeDamage(ArenaManager::GetInstance()->GetPlayerData()->damage);

		if (states->actualAction == "Idle")
		{
			Aggro();
		}
	}

}

void BotEnemy::Aggro()
{
	states->Transit("GetInRange");
}

void BotEnemy::Death()
{
	pZone->EnemyDeath();
	ArenaManager::GetItemManager()->CreateItemDropFor(ArenaManager::GetItemManager()->GetRandomItem(ItemData::ENEMY), m_pOwner->transform.GetWorldPosition());

	Enemy::Death();
}

void BotEnemy::FindPathToAlly()
{
	Graph<Tile>* graph = ArenaManager::GetMapLoader()->mp_currentGraph;

	Node<Tile>* currentNode = GetClosestNode(m_pOwner->transform.GetWorldPosition(), graph);
	Node<Tile>* target = GetClosestNode(pZone->GetClosestEnemy(m_pOwner->transform.GetWorldPosition(), m_pOwner)->pgameObject->transform.GetWorldPosition(), graph);
	if (currentNode == nullptr || target == nullptr)
	{
		return;
	}
	mp_path = FindPath<Tile>(currentNode, target, graph);
}