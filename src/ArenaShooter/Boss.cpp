#include "Boss.h"
#include "StateMachine.h"
#include "ArenaManager.h"
#include "LaserBullet.h"

#define MISSILE_HOMING_MAX_RANGE 50.f

// --- Missile Script --- //
void MissileHoming::Start() {
	homingStrength = 1.5f;
	player = ArenaManager::GetInstance()->GetPlayer();
	script = m_pOwner->GetScript<LaserBullet>();
}

void MissileHoming::Update() {
	if (player == nullptr || script == nullptr) {
		return;
	}

	gce::Vector3f32 delta = (player->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition());

	if (delta.Norm() >= MISSILE_HOMING_MAX_RANGE)
	{
		return;
	}

	delta = delta.Normalize();

	gce::Vector3f32 direction = gce::Vector3f32::Lerp(script->m_forward, delta, homingStrength * gce::GameManager::DeltaTime()).Normalize();
	script->m_forward = direction;
}

// --- Dash Hitbox Script --- //
void BossForcefield::CollisionEnter(gce::GameObject* other)
{
	if (hit)
	{
		return;
	}

	if (other->GetName() == "Wall")
	{
		mp_bossScript->states->Transit("Idle");
		mp_bossScript->m_invulnerable = false;
		hit = true;
		return;
	}

	if (damaging == false)
	{
		return;
	}

	Player* playerScript = other->GetScript<Player>();
	if (playerScript != nullptr)
	{
		playerScript->TakeDamage(mp_bossScript->m_dashDamage);
		hit = true;
	}
}


// --- Boss --- //

bool HasLowHealth(gce::GameObject* pMe)
{
	Boss* bossScript = pMe->GetScript<Boss>();

	if (bossScript->m_firstBarrierActivated == false && bossScript->currentHealth <= 0.66f * bossScript->maxHealth)
	{
		bossScript->m_firstBarrierActivated = true;
		return true;
	}

	else if (bossScript->m_secondBarrierActivated == false && bossScript->currentHealth <= 0.33f * bossScript->maxHealth)
	{
		bossScript->m_secondBarrierActivated = true;
		return true;
	}

	return false;
}

void StartBossIdle(gce::GameObject* pMe)
{
	Boss* bossScript = pMe->GetScript<Boss>();
	bossScript->m_idleChrono = 0.f;
}

void BossIdle(gce::GameObject* pMe)
{
	Boss* bossScript = pMe->GetScript<Boss>();
	bossScript->m_idleChrono += gce::GameManager::DeltaTime();

	if (bossScript->m_idleChrono >= bossScript->m_idleTime)
	{
		// Change pattern
		bossScript->ChangePattern();
	}
}

void BossDashStart(gce::GameObject* pMe)
{
	Boss* bossScript = pMe->GetScript<Boss>();
	bossScript->m_dashChrono = 0.f;
	bossScript->m_telegraphChrono = 0.f;

	bossScript->mp_forcefield->SetActive(true);
	bossScript->mp_dashHitboxScript->hit = false;
	bossScript->mp_dashHitboxScript->damaging = false;

	gce::Vector3f32 delta = ArenaManager::GetInstance()->GetPlayer()->transform.GetWorldPosition() - pMe->transform.GetWorldPosition();
	delta.y = 0;
	gce::Vector3f32 deltaDir = delta.Normalize();
	bossScript->m_dashDirection = deltaDir;
	pMe->transform.SetWorldRotation(gce::Quaternion::RotationMatrix(gce::Matrix::LookToLH({ 0, 0, 0 }, { -deltaDir.x, 0, deltaDir.z }, pMe->transform.GetWorldUp())));
}

void BossDashEnd(gce::GameObject* pMe)
{
	Boss* bossScript = pMe->GetScript<Boss>();
	bossScript->mp_forcefield->SetActive(false);
}

void BossDash(gce::GameObject* pMe)
{
	Boss* bossScript = pMe->GetScript<Boss>();
	float dt = gce::GameManager::DeltaTime();
	bossScript->m_telegraphChrono += dt;

	if (bossScript->m_telegraphChrono < bossScript->m_telegraphTime)
	{
		bossScript->mp_dashHitboxScript->damaging = false;
		return;
	}

	bossScript->mp_dashHitboxScript->damaging = true;
	bossScript->m_dashChrono += dt;
	pMe->transform.WorldTranslate(bossScript->m_dashDirection * dt * bossScript->m_dashSpeed);
	if (bossScript->m_dashChrono >= bossScript->m_dashTime)
	{
		// Start new dash
		bossScript->m_dashChrono = 0.f;
		bossScript->m_telegraphChrono = 0.f;

		bossScript->mp_dashHitboxScript->hit = false;
		bossScript->mp_dashHitboxScript->damaging = false;

		gce::Vector3f32 delta = ArenaManager::GetInstance()->GetPlayer()->transform.GetWorldPosition() - pMe->transform.GetWorldPosition();
		delta.y = 0;
		gce::Vector3f32 deltaDir = delta.Normalize();
		bossScript->m_dashDirection = deltaDir;
		pMe->transform.SetWorldRotation(gce::Quaternion::RotationMatrix(gce::Matrix::LookToLH({ 0, 0, 0 }, { -deltaDir.x, 0, deltaDir.z }, pMe->transform.GetWorldUp())));
	}
}

void BossShootStart(gce::GameObject* pMe)
{
	Boss* bossScript = pMe->GetScript<Boss>();
	bossScript->m_shotChrono = 0.f;
	bossScript->m_bulletShootingChrono = 0.f;
}

void BossShoot(gce::GameObject* pMe)
{
	Boss* bossScript = pMe->GetScript<Boss>();
	bossScript->m_shotChrono += gce::GameManager::DeltaTime();
	bossScript->m_bulletShootingChrono += gce::GameManager::DeltaTime();

	gce::Vector3f32 playerPos = ArenaManager::GetInstance()->GetPlayer()->transform.GetWorldPosition();
	gce::Vector3f32 delta = (playerPos - pMe->transform.GetWorldPosition());
	gce::Vector3f32 deltaDir = delta.Normalize();
	pMe->transform.SetWorldRotation(gce::Quaternion::RotationMatrix(gce::Matrix::LookToLH({ 0, 0, 0 }, { -deltaDir.x, 0, deltaDir.z }, pMe->transform.GetWorldUp())));

	if (bossScript->m_shotChrono >= bossScript->m_shotTime)
	{
		bossScript->m_shotChrono -= bossScript->m_shotTime;

		gce::GameObject& bullet = gce::GameObject::Create(*(ArenaManager::GetInstance()->GetScene()));
		gce::MeshRenderer& meshRenderer = *bullet.AddComponent<gce::MeshRenderer>();
		LaserBullet* bulletScript = bullet.AddScript<LaserBullet>();
		bulletScript->bulletSpeed = 60;
		bullet.transform.SetWorldPosition(pMe->transform.GetWorldPosition() + pMe->transform.GetLocalForward());
		pMe->AddChild(bullet);
		bullet.transform.LocalTranslate(bossScript->m_bulletOffset);
		pMe->RemoveChild(bullet);
		bulletScript->m_forward = (playerPos - bullet.transform.GetWorldPosition()).Normalize();
		bulletScript->m_damage = 20;

		meshRenderer.SetGeometry(gce::SHAPES.SPHERE);
		meshRenderer.SetAlbedoTexture(ResourceManager::GetTexture("res/Textures/UniformPink.png"));

		bullet.AddComponent<gce::SphereCollider>()->isTrigger = true;
		bullet.AddComponent<gce::PhysicComponent>()->SetGravityScale(0);

		bullet.transform.SetWorldScale({ 0.5f , 0.5f , 0.5f });
	}

	if (bossScript->m_bulletShootingChrono >= bossScript->m_bulletShootingTime)
	{
		// Go back to Idle
		bossScript->states->Transit("Idle");
	}
}

void BossMissileStart(gce::GameObject* pMe)
{
	Boss* bossScript = pMe->GetScript<Boss>();
	bossScript->m_missileChrono = 0.f;
	bossScript->m_missileShootingChrono = 0.f;
	bossScript->m_missilesShot = 0;
}

void BossMissile(gce::GameObject* pMe)
{
	Boss* bossScript = pMe->GetScript<Boss>();
	bossScript->m_missileChrono += gce::GameManager::DeltaTime();
	bossScript->m_missileShootingChrono += gce::GameManager::DeltaTime();

	gce::Vector3f32 playerPos = ArenaManager::GetInstance()->GetPlayer()->transform.GetWorldPosition();
	gce::Vector3f32 delta = (playerPos - pMe->transform.GetWorldPosition());
	gce::Vector3f32 deltaDir = delta.Normalize();
	pMe->transform.SetWorldRotation(gce::Quaternion::RotationMatrix(gce::Matrix::LookToLH({ 0, 0, 0 }, { -deltaDir.x, 0, deltaDir.z }, pMe->transform.GetWorldUp())));

	if (bossScript->m_missilesShot < bossScript->m_missileAmount && bossScript->m_missileChrono >= bossScript->m_missileTime)
	{
		bossScript->m_missileChrono -= bossScript->m_missileTime;
		bossScript->m_missilesShot++;

		gce::GameObject& bullet = gce::GameObject::Create(*(ArenaManager::GetInstance()->GetScene()));
		gce::MeshRenderer& meshRenderer = *bullet.AddComponent<gce::MeshRenderer>();
		LaserBullet* bulletScript = bullet.AddScript<LaserBullet>();
		bulletScript->bulletSpeed = 20;
		bullet.transform.SetWorldPosition(pMe->transform.GetWorldPosition() + pMe->transform.GetLocalForward());
		pMe->AddChild(bullet);
		bullet.transform.LocalTranslate(bossScript->m_missileOffset);
		pMe->RemoveChild(bullet);
		bulletScript->m_forward = ((playerPos + gce::Vector3f32(rand() % 16 - 7.5f, rand() % 16 - 7.5f, rand() % 16 - 7.5f)) - bullet.transform.GetWorldPosition()).Normalize();
		bulletScript->m_damage = 35;

		bullet.AddScript<MissileHoming>();

		meshRenderer.SetGeometry(gce::SHAPES.SPHERE);
		meshRenderer.SetAlbedoTexture(ResourceManager::GetTexture("res/Textures/UniformOrange.png"));

		bullet.AddComponent<gce::SphereCollider>()->isTrigger = true;
		bullet.AddComponent<gce::PhysicComponent>()->SetGravityScale(0);

		bullet.transform.SetWorldScale({ 0.5f , 0.5f , 0.5f });
	}

	if (bossScript->m_missileShootingChrono >= bossScript->m_missileShootingTime)
	{
		// Go back to Idle
		bossScript->states->Transit("Idle");
	}
}

void Boss::Start()
{
	m_pOwner->transform.SetWorldScale({ 2, 2, 2 });

	m_idleTime = 2.f;
	m_shotTime = 0.5f;
	m_bulletShootingTime = 5.f;
	m_missileShootingTime = 4.f;
	m_missileTime = 0.15f;
	m_missileAmount = 8;

	m_telegraphTime = 0.5f;
	m_dashTime = 2.f;
	m_dashSpeed = 35.f;

	Enemy::Start();

	armor = 100;
	maxHealth = 750;
	currentHealth = maxHealth;

	m_bulletOffset = gce::Vector3f32(2.4f, 1.3f, 0);
	m_missileOffset = gce::Vector3f32(-2.4f, 1.2f, 0);

	states = gce::GameManager::GetStatesSystem().CreateStateMachine(m_pOwner);
	states->AddAction("Idle", StartBossIdle, BossIdle, nullptr);
	states->AddAction("Dash", BossDashStart, BossDash, BossDashEnd); // On 66% and 33% health

	states->AddAction("Shoot", BossShootStart, BossShoot, nullptr);
	states->AddAction("Laser", nullptr, nullptr, nullptr);
	states->AddAction("Missile", BossMissileStart, BossMissile, nullptr);

	states->Transit("Idle");

	gce::StateMachine::Transition hpPhaseChange;
	hpPhaseChange.conditions.PushBack(gce::StateMachine::Condition(HasLowHealth));
	hpPhaseChange.target = "Dash";
	states->AddTransition(hpPhaseChange);

	gce::Scene* pscene = ArenaManager::GetInstance()->GetScene();

	mp_forcefield = &gce::GameObject::Create(*pscene);
	m_pOwner->AddChild(*mp_forcefield);
	gce::MeshRenderer* forcefieldRenderer = mp_forcefield->AddComponent<gce::MeshRenderer>();
	forcefieldRenderer->SetGeometry(ArenaManager::GetResourceManager()->GetGeometry("res/Models/MobBossForcefield.obj"));
	forcefieldRenderer->SetAlbedoTexture(ArenaManager::GetResourceManager()->GetTexture("res/Textures/MobBossForcefield_Albedo.png"));
	gce::BoxCollider* forcefieldCollider = mp_forcefield->AddComponent<gce::BoxCollider>();
	forcefieldCollider->isTrigger = true; 
	mp_dashHitboxScript = mp_forcefield->AddScript<BossForcefield>();
	mp_dashHitboxScript->mp_bossScript = this;
	mp_forcefield->SetActive(false);
	mp_forcefield->transform.SetLocalPosition({0, 0, 0});
	mp_forcefield->transform.SetLocalScale({1, 1, 1});

	gce::Vector2f32 screen = { (float)gce::GameManager::GetWindowParam().width, (float)gce::GameManager::GetWindowParam().height };

	m_healthbar.background = &CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/BossBarBackground.png"), { screen.x * 0.5f, screen.y * 0.9f}, {2, 2});
	m_healthbar.bar = &CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/BossBarFill.png"), { screen.x * 0.5f, screen.y * 0.9f }, { 2, 2 });
	m_healthbar.top = &CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/BossBarTop.png"), { screen.x * 0.5f, screen.y * 0.875f }, { 2, 2 });
}

void Boss::Update()
{
	gce::UiImage* bar = m_healthbar.bar->GetComponent<gce::UiImage>();

	bar->size = { 400 * (currentHealth / maxHealth), 1 };
	bar->rectImage->left = bar->position.x - (400 * (currentHealth / maxHealth)) / 2;
	bar->rectImage->right = bar->position.x + (400 * (currentHealth / maxHealth)) / 2;

	float deltaTime = gce::GameManager::DeltaTime();

	if (isDying) {
		currentDeathFeedbackTimer += deltaTime;
		if (currentDeathFeedbackTimer >= deathFeedbackTimer)
		{
			ArenaManager::GetInstance()->SetState("Win");
			m_pOwner->SetActive(false);
		}
	}

}

void Boss::Destroy()
{
	m_healthbar.background->Destroy();
	m_healthbar.bar->Destroy();
	m_healthbar.top->Destroy();
}

void Boss::ChangePattern()
{
	switch (rand() % 2)
	{
	case 0:
		states->Transit("Shoot");
		break;
	case 1:
		states->Transit("Missile");
		break;
	}
	
}
