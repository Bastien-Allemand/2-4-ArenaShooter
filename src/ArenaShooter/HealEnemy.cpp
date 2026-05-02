#include "HealEnemy.h"
#include <Engine/StateMachine.h>
#include "ArenaManager.h"
#include "Maploader.h"
#include "Pathfinding.hpp"
#include "MeleeEnemy.h"
#include "LaserEnemy.h"
#include "Zone.h"


void HealStart(gce::GameObject* pMe)
{
	HealEnemy* script = pMe->GetScript<HealEnemy>(); 
	script->m_healChrono = 0;
	script->mp_healVisual->SetActive(true);
}

void HealEnd(gce::GameObject* pMe)
{
	HealEnemy* script = pMe->GetScript<HealEnemy>();
	script->mp_healVisual->SetActive(false);
}

void HealHeal(gce::GameObject* pMe)
{
	HealEnemy* script = pMe->GetScript<HealEnemy>();

	if (script->mp_healTarget != nullptr)
	{
		delete script->mp_healTarget;
	}
	script->mp_healTarget = script->pZone->GetClosestEnemy(pMe->transform.GetWorldPosition(), pMe);

	if (script->mp_healTarget->pgameObject == nullptr)
	{
		script->states->Transit("Idle");
		return;
	}

	gce::Vector3f32 delta = (script->mp_healTarget->pgameObject->transform.GetWorldPosition() - pMe->transform.GetWorldPosition());
	float distance = delta.Norm();
	script->mp_healVisual->transform.SetLocalScale({0.25f, distance, 0.25f });
	script->mp_healVisual->transform.SetLocalPosition({0, 0, distance / 2 });
	script->mp_healVisual->transform.SetLocalRotation({gce::PI/2, 0, 0});

	if (distance >= script->attackActivationRange || script->mp_healTarget->obstructed == true)
	{
		script->states->Transit("GetInRange");
	}

	pMe->transform.SetWorldRotation(gce::Quaternion::RotationMatrix(gce::Matrix::LookToLH({ 0, 0, 0 }, { -delta.x, delta.y, delta.z }, pMe->transform.GetWorldUp())));

	float dt = gce::GameManager::DeltaTime();
	script->m_healChrono += dt;
	while (script->m_healChrono >= script->m_healTime)
	{
		script->m_healChrono -= script->m_healTime;
		BotEnemy* targetScript = script->mp_healTarget->pgameObject->GetScript<MeleeEnemy>();
		targetScript = targetScript == nullptr ? script->mp_healTarget->pgameObject->GetScript<LaserEnemy>() : targetScript;
		targetScript = targetScript == nullptr ? script->mp_healTarget->pgameObject->GetScript<HealEnemy>() : targetScript;

		targetScript->Heal(script->damage);
	}
}

void HealStartRange(gce::GameObject* pMe)
{
	HealEnemy* script = pMe->GetScript<HealEnemy>();
	if (script->mp_healTarget != nullptr)
	{
		delete script->mp_healTarget;
	}
	script->mp_healTarget = script->pZone->GetClosestEnemy(pMe->transform.GetWorldPosition(), pMe);

	script->FindPathToAlly();

	script->mp_currentNode = script->mp_path->m_list.GetCurrent();
	script->mp_nextNode = script->mp_path->m_list.GetForward();

	script->m_direction = (script->mp_currentNode->GetData()->GetPosition() - pMe->transform.GetWorldPosition());

	script->m_distanceToNext = (script->m_direction).Norm();
	script->m_distanceTravelled = 0;

	script->m_direction = script->m_direction.Normalize();

}
void HealGetRange(gce::GameObject* pMe)
{
	HealEnemy* script = pMe->GetScript<HealEnemy>();

	if (script->mp_healTarget != nullptr)
	{
		delete script->mp_healTarget;
	}
	script->mp_healTarget = script->pZone->GetClosestEnemy(pMe->transform.GetWorldPosition(), pMe);

	if (script->mp_healTarget->pgameObject == nullptr)
	{
		return;
	}

	gce::Vector3f32 delta = (script->mp_healTarget->pgameObject->transform.GetWorldPosition() - pMe->transform.GetWorldPosition());
	delta *= gce::Vector3f32(1, 0, 1);

	if (delta.Norm() <= script->attackActivationRange && script->mp_healTarget->obstructed == false)
	{
		script->states->Transit("Attack");
		return;
	}

	float dt = gce::GameManager::DeltaTime();
	gce::Vector3f32 deltaDir = delta.Normalize();

	pMe->transform.SetWorldRotation(gce::Quaternion::RotationMatrix(gce::Matrix::LookToLH({ 0, 0, 0 }, { -deltaDir.x, 0, deltaDir.z }, pMe->transform.GetWorldUp())));

	float toTravel = dt * script->moveSpeed;
	script->m_distanceTravelled += toTravel;
	pMe->transform.WorldTranslate(script->m_direction * toTravel);
	if (script->m_distanceTravelled >= script->m_distanceToNext)
	{
		script->FindPathToAlly();

		script->mp_path->m_list.Forward();

		script->mp_currentNode = script->mp_path->m_list.GetCurrent();
		script->mp_nextNode = script->mp_path->m_list.GetForward();

		script->m_distanceToNext = (script->mp_nextNode->GetData()->GetPosition() - script->mp_currentNode->GetData()->GetPosition()).Norm();
		script->m_distanceTravelled = 0;

		script->m_direction = (script->mp_nextNode->GetData()->GetPosition() - script->mp_currentNode->GetData()->GetPosition()).Normalize();
	}
}

void HealEnemy::Start()
{
	BotEnemy::Start();

	moveSpeed = 16;
	shotCooldown = 1;
	damage = 20;
	m_healTime = 0.5f;
	attackActivationRange = 30;
	states->SetOnBeginAction("Attack", HealStart);
	states->SetOnUpdateAction("Attack", HealHeal);
	states->SetOnEndAction("Attack", HealEnd);
	states->SetOnBeginAction("GetInRange", HealStartRange);
	states->SetOnUpdateAction("GetInRange", HealGetRange);

	mp_healVisual = &gce::GameObject::Create(*m_pOwner->GetScene());
	m_pOwner->AddChild(*mp_healVisual);
	gce::MeshRenderer* effectRenderer = mp_healVisual->AddComponent<gce::MeshRenderer>();
	effectRenderer->SetGeometry(gce::SHAPES.CYLINDER);
	effectRenderer->SetAlbedoTexture(ArenaManager::GetResourceManager()->GetTexture("res/Textures/UniformGreen.png"));
	mp_healVisual->transform.SetLocalRotation({90, 0, 0});
	mp_healVisual->SetActive(false);

	mp_healTarget = nullptr;

}

void HealEnemy::Update()
{
	//std::cout << m_distanceTravelled << " / " << m_distanceToNext << std::endl;
	BotEnemy::Update();
}

void HealEnemy::Destroy()
{
	delete mp_healTarget;
	mp_healVisual->Destroy();
}