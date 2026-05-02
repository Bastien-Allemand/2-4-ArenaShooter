#include "Player.h"
#include "ArenaManager.h"
#include <Tweens.hpp>
#include "GameUtils.h"

void Player::Reset()
{
	SetStats(m_baseStats);
	ammoLeft = ammoCapacity;
	currentHealth = maxHealth;
	m_pOwner->transform.SetWorldPosition({ 0, 0, 0 });
	m_pOwner->transform.SetWorldRotation({ 0, 0, 0 });
}

void Player::ActivateUI(bool active)
{
	mp_healthDisplay->SetActive(active);
	mp_ammoDisplay->SetActive(active);
	mp_ammoIcon->SetActive(active);
	mp_healthIcon->SetActive(active);
	mp_crosshair->SetActive(active);
}


void Player::Awake() 
{
	m_baseStats.maxHealth = 100; //placeholder value
	m_baseStats.ammoCapacity = 10;
	m_baseStats.atkSpeed = 1;

	m_baseStats.armor = 5; //placeholder value
	m_baseStats.damage = 25; //placeholder value

	m_baseStats.healthRegen = 1;
	m_baseStats.moveSpeed = 10;

	SetStats(m_baseStats);

	shotCooldown = 1;
	ammoLeft = ammoCapacity;
	currentHealth = maxHealth;

	// ---- UI ---- //
	gce::Vector2f32 screenSize = { (float)gce::GameManager::GetWindowParam().width, (float)gce::GameManager::GetWindowParam().height };
	gce::Vector2f32 center = { screenSize.x / 2, screenSize.y / 2 };
	gce::Vector2f32 posUi = center;
	gce::Vector2f32 size = { 4 ,4 };

	mp_healthDisplay = gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()).AddComponent<gce::TextRenderer>();
	mp_healthDisplay->rectPosF = new gce::RectanglePosF(62, 382, 162, 382);
	mp_healthDisplay->text = L"";
	mp_healthDisplay->scale = 1.5f;
	mp_healthDisplay->pFont = ArenaManager::GetResourceManager()->GetFont(L"Jersey 10");
	mp_ammoDisplay = gce::GameObject::Create(*ArenaManager::GetInstance()->GetScene()).AddComponent<gce::TextRenderer>();
	mp_ammoDisplay->rectPosF = new gce::RectanglePosF(685, 382, 785, 382);
	mp_ammoDisplay->text = L"";
	mp_ammoDisplay->scale = 1.5f;
	mp_ammoDisplay->pFont = ArenaManager::GetResourceManager()->GetFont(L"Jersey 10");

	mp_crosshair = &CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/crosshair.png"), center, {2, 2});

	mp_healthIcon = &CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/HealthPoints.png"), {screenSize.x * 0.02f, screenSize.y * 0.98f}, {4, 4}, {0.f, 1.f});
	mp_ammoIcon = &CreateImage(ArenaManager::GetResourceManager()->GetBitMapBrush("res/UI/ammo.png"), {screenSize.x * 0.98f, screenSize.y * 0.98f}, {4, 4}, {1.f, 1.f});
}

void Player::Start() 
{
}

void Player::Update()
{
	float deltatime = gce::GameManager::DeltaTime();

	mp_healthDisplay->text = std::to_wstring((int)currentHealth);
	mp_ammoDisplay->text = std::to_wstring(ammoLeft);

	if (currentRegenCooldown > 0) {
		currentRegenCooldown -= deltatime;
	}

	else
	{
		currentRegenCooldown = regenCooldown;
		Heal(healthRegen);
	}

	if (currentShotCooldown > 0) {
		currentShotCooldown -= deltatime;
	}
}

void Player::Death() 
{
	ArenaManager::GetInstance()->SetState("GameOver");
}


void Player::UpdateStats()
{
	PlayerStats totalStats = m_baseStats;
	std::map<ItemData*, int16> itemMap = ArenaManager::GetInstance()->GetItemManager()->m_itemMap;

	for (auto item : itemMap)
	{
		for (int i = 0; i < item.second; i++)
		{
			totalStats += item.first->GetStats();
		}
	}

	SetStats(totalStats);
}

void Player::TakeDamage(float damageValue) {
	float damageTaken;
	if (armor > -99) {
		damageTaken = damageValue / (1.0f + armor / 100.0f);
	}
	else {
		damageTaken = damageValue / (1.0f - 99.0f / 100.0f);
	}
	currentHealth -= damageTaken;


	gce::Tween<gce::Vector3f32>* damageFeedback = nullptr;

	damageFeedback = &gce::TweenSystem::Create<gce::Vector3f32>(gce::Vector3f32(0, 0, 0), gce::Vector3f32(0, 0, -0.15), gce::Tweens::EaseIn);

	damageFeedback->StartDuration(damageFeedbackTimer, Function<void(gce::GameTransform::*)(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalRotation, &m_pOwner->transform));
	hasDamaged = true;

	std::cout << "Damage taken : " << damageTaken << std::endl;
	if (currentHealth <= 0) {
		Death();
	}
}

void Player::SetStats(PlayerStats const& _stats)
{
	// Defensive
	maxHealth = _stats.maxHealth;
	healthRegen = _stats.healthRegen;
	armor = _stats.armor;
	// Offensive
	damage = _stats.damage;
	ammoCapacity = _stats.ammoCapacity;
	atkSpeed = _stats.atkSpeed;
	// Movement
	moveSpeed = _stats.moveSpeed;
	runSpeedValue = _stats.runSpeedValue;
	crouchSlowValue = _stats.crouchSlowValue;
}