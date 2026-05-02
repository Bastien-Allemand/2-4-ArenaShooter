#include "Enemy.h"
#include "Bullet.h"
#include "ArenaManager.h"
#include "Maploader.h"
#include "Pathfinding.hpp"
#include "Tweens.hpp"
#include "StateMachine.h"

void Enemy::CollisionEnter(gce::GameObject* other)
{
	if (other->HasScript<Bullet>()) {
		TakeDamage(ArenaManager::GetInstance()->GetPlayerData()->damage);
	}
}

void Enemy::Start() {
	states = gce::GameManager::GetStatesSystem().CreateStateMachine(m_pOwner);
	states->AddAction("DeathIdle", nullptr, nullptr, nullptr);
}

void Enemy::Death() {
	gce::Tween<gce::Vector3f32>* deathFeedback = nullptr;
	deathFeedback = &gce::TweenSystem::Create<gce::Vector3f32>(gce::Vector3f32(0, 0, 0),
		gce::Vector3f32(gce::PI/2, 0, -0.02), gce::Tweens::EaseIn);

	deathFeedback->StartDuration(deathFeedbackTimer, Function<void(gce::GameTransform::*)
		(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalRotation, &m_pOwner->transform));

	states->Transit("DeathIdle");

	isDying = true;
	m_pOwner->GetComponent<gce::BoxCollider>()->SetActive(false);
}

void Enemy::Update() 
{
	float deltaTime = gce::GameManager::DeltaTime();

	if (isDying) {
		currentDeathFeedbackTimer += deltaTime;
		if (currentDeathFeedbackTimer >= deathFeedbackTimer) {
			m_pOwner->SetActive(false);
		}
	}

}

void Enemy::FindPathToPlayer()
{
	Graph<Tile>* graph = ArenaManager::GetMapLoader()->mp_currentGraph;
	// Find closest node
	Node<Tile>* currentNode = GetClosestNode(m_pOwner->transform.GetWorldPosition(), graph);
	Node<Tile>* target = GetClosestNode(ArenaManager::GetInstance()->GetPlayer()->transform.GetWorldPosition(), graph);

	if (currentNode == nullptr || target == nullptr)
	{
		return;
	}

	mp_path = FindPath<Tile>(currentNode, target, graph);
	redirecting = true;
}

