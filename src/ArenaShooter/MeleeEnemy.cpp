#include "MeleeEnemy.h"
#include <Engine/StateMachine.h>
#include "ArenaManager.h"

DECLARE_SCRIPT(DamageArea, ScriptFlag::CollisionEnter)

float damage = 10;
bool hit = false;

void CollisionEnter(gce::GameObject* other)
{
	if (hit)
	{
		return;
	}

	Player* playerScript = other->GetScript<Player>();
	if (playerScript != nullptr)
	{
		playerScript->TakeDamage(damage);
		hit = true;
	}
}

END_SCRIPT


void BotRoam(gce::GameObject* pMe)
{
	// TODO: everything
	MeleeEnemy* script = pMe->GetScript<MeleeEnemy>();
	float dt = gce::GameManager::DeltaTime();

	// Increase chrono
	script->m_roamChrono += dt * script->shotCooldown;
	if (script->m_roamChrono >= script->m_roamTime)
	{
		script->states->Transit("GetInRange");
	}
}

void MeleeTelegraphStart(gce::GameObject* pMe)
{
	MeleeEnemy* script = pMe->GetScript<MeleeEnemy>();
	script->m_telegraphChrono = 0;
	gce::Vector3f32 delta = ArenaManager::GetInstance()->GetPlayer()->transform.GetWorldPosition() - pMe->transform.GetWorldPosition();
	delta.y = 0;
	gce::Vector3f32 deltaDir = delta.Normalize();
	script->m_targetDirection = deltaDir;
	pMe->transform.SetWorldRotation(gce::Quaternion::RotationMatrix(gce::Matrix::LookToLH({ 0, 0, 0 }, { -deltaDir.x, deltaDir.y, deltaDir.z }, pMe->transform.GetWorldUp())));
}

void MeleeTelegraph(gce::GameObject* pMe)
{
	MeleeEnemy* script = pMe->GetScript<MeleeEnemy>();
	float dt = gce::GameManager::DeltaTime();

	// Don't move
	
	// Increase chrono
	script->m_telegraphChrono += dt * script->shotCooldown;
	if (script->m_telegraphChrono >= script->m_telegraphTime)
	{
		script->states->Transit("Attack");
	}
}

void MeleeAttackStart(gce::GameObject* pMe)
{
	MeleeEnemy* script = pMe->GetScript<MeleeEnemy>();
	script->m_attackChrono = 0;
	script->mp_attackArea->SetActive(true);
	script->mp_attackArea->GetScript<DamageArea>()->hit = false;
}

void MeleeAttackEnd(gce::GameObject* pMe)
{
	MeleeEnemy* script = pMe->GetScript<MeleeEnemy>();
	script->mp_attackArea->SetActive(false);
}

void MeleeAttack(gce::GameObject* pMe)
{	
	MeleeEnemy* script = pMe->GetScript<MeleeEnemy>();
	float dt = gce::GameManager::DeltaTime();

	pMe->transform.WorldTranslate(script->m_targetDirection * dt * script->m_dashSpeed);

	script->m_attackChrono += dt;
	if (script->m_attackChrono >= script->m_attackTime)
	{
		script->states->Transit("GetInRange");
	}
}

void MeleeStartRange(gce::GameObject* pMe)
{
	MeleeEnemy* script = pMe->GetScript<MeleeEnemy>();

	script->FindPathToPlayer();

	script->mp_currentNode = script->mp_path->m_list.GetCurrent();
	script->mp_nextNode = script->mp_path->m_list.GetForward();

	script->m_direction = (script->mp_currentNode->GetData()->GetPosition() - pMe->transform.GetWorldPosition());

	script->m_distanceToNext = (script->m_direction).Norm();
	script->m_distanceTravelled = 0;

	script->m_direction = script->m_direction.Normalize();
}
void MeleeGetRange(gce::GameObject* pMe)
{
	MeleeEnemy* script = pMe->GetScript<MeleeEnemy>();
	gce::Vector3f32 playerPos = ArenaManager::GetInstance()->GetPlayer()->transform.GetWorldPosition();
	gce::Vector3f32 delta = (playerPos - pMe->transform.GetWorldPosition());
	delta *= gce::Vector3f32(1, 0, 1);

	if (delta.Norm() <= script->attackActivationRange)
	{
		script->states->Transit("Telegraph");
		return;
	}

	float dt = gce::GameManager::DeltaTime();

	if (script->mp_currentNode == script->mp_nextNode)
	{
		script->redirecting = false;
		return;
	}

	gce::Vector3f32 deltaDir = delta.Normalize();
	pMe->transform.SetWorldRotation(gce::Quaternion::RotationMatrix(gce::Matrix::LookToLH({ 0, 0, 0 }, { -deltaDir.x, deltaDir.y, deltaDir.z }, pMe->transform.GetWorldUp())));

	float toTravel = dt * script->moveSpeed;
	script->m_distanceTravelled += toTravel;
	pMe->transform.WorldTranslate(script->m_direction * toTravel);
	if (script->m_distanceTravelled >= script->m_distanceToNext)
	{
		script->FindPathToPlayer();

		script->mp_path->m_list.Forward();

		script->mp_currentNode = script->mp_path->m_list.GetCurrent();
		script->mp_nextNode = script->mp_path->m_list.GetForward();

		script->m_distanceToNext = (script->mp_nextNode->GetData()->GetPosition() - script->mp_currentNode->GetData()->GetPosition()).Norm();
		script->m_distanceTravelled = 0;

		script->m_direction = (script->mp_nextNode->GetData()->GetPosition() - script->mp_currentNode->GetData()->GetPosition()).Normalize();
	}
}

void MeleeEnemy::Start()
{
	BotEnemy::Start();

	moveSpeed = 8;
	shotCooldown = 1;
	m_dashSpeed = 30;
	damage = 50;
	armor = 30;
	maxHealth = 60;
	currentHealth = maxHealth;

	attackActivationRange = 20;
	states->SetOnBeginAction("Attack", MeleeAttackStart);
	states->SetOnUpdateAction("Attack", MeleeAttack);
	states->SetOnEndAction("Attack", MeleeAttackEnd);
	states->SetOnUpdateAction("Roam", BotRoam);
	states->SetOnBeginAction("GetInRange", MeleeStartRange);
	states->SetOnUpdateAction("GetInRange", MeleeGetRange);
	states->SetOnBeginAction("Telegraph", MeleeTelegraphStart);
	states->SetOnUpdateAction("Telegraph", MeleeTelegraph);

	mp_attackArea = &gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene());
	mp_attackArea->AddComponent<gce::BoxCollider>()->localScale = {4, 4, 4}; // Default is 1, 1, 1
	mp_attackArea->AddScript<DamageArea>()->damage = damage;
	mp_attackArea->SetActive(false);
	m_pOwner->AddChild(*mp_attackArea);
	mp_attackArea->GetComponent<gce::BoxCollider>()->isTrigger = true;
	mp_attackArea->transform.SetLocalPosition({0, 0, 0});
}