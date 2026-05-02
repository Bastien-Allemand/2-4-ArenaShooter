#include "LaserEnemy.h"
#include <Engine/StateMachine.h>
#include "ArenaManager.h"
#include "LaserBullet.h"

void LaserBotRoam(gce::GameObject* pMe)
{
	LaserEnemy* script = pMe->GetScript<LaserEnemy>();
	float dt = gce::GameManager::DeltaTime();

	// Increase chrono
	script->m_roamChrono += dt * script->shotCooldown;
	if (script->m_roamChrono >= script->m_roamTime)
	{
		script->states->Transit("GetInRange");
	}
}

void LaserTelegraphStart(gce::GameObject* pMe)
{
	LaserEnemy* script = pMe->GetScript<LaserEnemy>();
	script->m_telegraphChrono = 0;
	gce::Vector3f32 delta = ArenaManager::GetInstance()->GetPlayer()->transform.GetWorldPosition() - (pMe->transform.GetWorldPosition() + pMe->transform.GetLocalForward());
	delta.y = 0;
	gce::Vector3f32 deltaDir = delta.Normalize();
	script->m_targetDirection = deltaDir;

	pMe->transform.SetWorldRotation(gce::Quaternion::RotationMatrix(gce::Matrix::LookToLH({ 0, 0, 0 }, { -deltaDir.x, deltaDir.y, deltaDir.z }, pMe->transform.GetWorldUp())));
}

void LaserTelegraph(gce::GameObject* pMe)
{
	LaserEnemy* script = pMe->GetScript<LaserEnemy>();
	float dt = gce::GameManager::DeltaTime();

	// Increase chrono
	script->m_telegraphChrono += dt * script->shotCooldown;
	if (script->m_telegraphChrono >= script->m_telegraphTime)
	{
		script->states->Transit("Attack");
	}
}

void LaserAttackStart(gce::GameObject* pMe)
{
	LaserEnemy* script = pMe->GetScript<LaserEnemy>();
	gce::GameObject& bullet = gce::GameObject::Create(*(ArenaManager::GetInstance()->GetScene()));
	gce::MeshRenderer& meshRenderer = *bullet.AddComponent<gce::MeshRenderer>();
	LaserBullet* bulletScript = bullet.AddScript<LaserBullet>();
	bulletScript->m_forward = script->m_targetDirection;
	bulletScript->m_damage = script->damage;

	meshRenderer.SetGeometry(gce::SHAPES.SPHERE);
	meshRenderer.SetAlbedoTexture(ResourceManager::GetTexture("res/Textures/UniformPink.png"));
	bullet.transform.SetWorldPosition(pMe->transform.GetWorldPosition() + pMe->transform.GetLocalForward());

	bullet.AddComponent<gce::SphereCollider>()->isTrigger = true;
	bullet.AddComponent<gce::PhysicComponent>()->SetGravityScale(0);

	bullet.transform.SetWorldScale({ 0.5f , 0.5f , 0.5f });

	script->m_attackChrono = 0;
}

void LaserAttackEnd(gce::GameObject* pMe)
{
	
}

void LaserAttack(gce::GameObject* pMe)
{
	LaserEnemy* script = pMe->GetScript<LaserEnemy>();
	float dt = gce::GameManager::DeltaTime();

	script->m_attackChrono += dt;
	if (script->m_attackChrono >= script->m_attackTime)
	{
		script->states->Transit("Roam");
	}
}

void LaserStartRange(gce::GameObject* pMe)
{
	LaserEnemy* script = pMe->GetScript<LaserEnemy>();

	script->FindPathToPlayer();

	script->mp_currentNode = script->mp_path->m_list.GetCurrent();
	script->mp_nextNode = script->mp_path->m_list.GetForward();

	script->m_direction = (script->mp_currentNode->GetData()->GetPosition() - pMe->transform.GetWorldPosition());

	script->m_distanceToNext = (script->m_direction).Norm();
	script->m_distanceTravelled = 0;

	script->m_direction = script->m_direction.Normalize();

}
void LaserGetRange(gce::GameObject* pMe)
{
	LaserEnemy* script = pMe->GetScript<LaserEnemy>();
	gce::Vector3f32 playerPos = ArenaManager::GetInstance()->GetPlayer()->transform.GetWorldPosition();
	gce::Vector3f32 delta = (playerPos - pMe->transform.GetWorldPosition());
	delta *= gce::Vector3f32(1, 0, 1);

	if (delta.Norm() <= script->attackActivationRange)
	{
		script->states->Transit("Telegraph");
		return;
	}

	float dt = gce::GameManager::DeltaTime();
	gce::Vector3f32 deltaDir = delta.Normalize();

	if (script->mp_currentNode == script->mp_nextNode)
	{
		script->redirecting = false;
		return;
	}

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

void LaserEnemy::Start()
{
	BotEnemy::Start();

	moveSpeed = 6;
	shotCooldown = 1;

	damage = 50;
	armor = 20;
	maxHealth = 40;
	currentHealth = maxHealth;
	attackActivationRange = 35;

	states->SetOnBeginAction("Attack", LaserAttackStart);
	states->SetOnUpdateAction("Attack", LaserAttack);
	states->SetOnEndAction("Attack", LaserAttackEnd);

	states->SetOnUpdateAction("Roam", LaserBotRoam);
	states->SetOnBeginAction("GetInRange", LaserStartRange);
	states->SetOnUpdateAction("GetInRange", LaserGetRange);
	states->SetOnBeginAction("Telegraph", LaserTelegraphStart);
	states->SetOnUpdateAction("Telegraph", LaserTelegraph);
}