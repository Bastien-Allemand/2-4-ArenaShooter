#include "PlayerController.h"
#include "Utils.h"
#include <Tweens.hpp>
#include "../Engine/GameManager.h"

void PlayerController::Awake() {
	playerData = ArenaManager::GetInstance()->GetPlayerData();
}

void PlayerController::Start() {
	
}

void PlayerController::InitializeCam()
{
	mp_playerCamera->transform.SetLocalPosition(gce::Vector3f32(0, m_cameraOffset, 0));
	mp_playerCamera->transform.SetLocalRotation(gce::Vector3f32(0, 0, 0));
}

void PlayerController::Shoot() {

	if (playerData->ammoLeft > 0 and playerData->currentShotCooldown <= 0 && not hasReload){

		gce::GameObject& bullet = gce::GameObject::Create(*(m_pOwner->GetScene()));
		gce::MeshRenderer& meshRenderer2 = *bullet.AddComponent<gce::MeshRenderer>();
		bullet.AddScript<Bullet>();

		meshRenderer2.SetGeometry(gce::SHAPES.SPHERE);
		meshRenderer2.SetAlbedoTexture(ResourceManager::GetTexture("res/Textures/UniformBlue.png"));
		bullet.transform.SetWorldRotation(mp_playerCamera->transform.GetWorldRotation());
		bullet.transform.SetWorldPosition(mp_playerCamera->transform.GetWorldPosition() + mp_playerCamera->transform.GetLocalForward());
		//bullet.transform.LocalTranslate(gce::Vector3f32(0.5, 0.5, 0.5));

		bullet.AddComponent<gce::SphereCollider>()->isTrigger = true;
		bullet.AddComponent<gce::PhysicComponent>()->SetGravityScale(0);

		ArenaManager::GetInstance()->GetItemManager()->OnShoot(&bullet);

		playerData->ammoLeft -= 1;
		playerData->currentShotCooldown = playerData->shotCooldown * (1 - playerData->atkSpeed / 100);
		if (playerData->currentShotCooldown > 0.5) {
			gunFeedbackTimer = playerData->currentShotCooldown / 10;
		}
		else {
			gunFeedbackTimer = 0.05;
		}
		
		gce::Tween<gce::Vector3f32>* gunFeedback = nullptr;
		gce::Tween<gce::Vector3f32>* gunFeedbackRotate = nullptr;

		gunFeedback = &gce::TweenSystem::Create<gce::Vector3f32>(mp_playerGun->transform.GetLocalPosition(),
			mp_playerGun->transform.GetLocalPosition() + gce::Vector3f32(0, 0, -0.2), gce::Tweens::EaseIn);

		gunFeedback->StartDuration(gunFeedbackTimer, Function<void(gce::GameTransform::*)(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalPosition, &mp_playerGun->transform));
		hasShot = true;

		gunFeedbackRotate = &gce::TweenSystem::Create<gce::Vector3f32>(gce::Vector3f32(0, 0, 0), gce::Vector3f32(-0.2, 0, 0), gce::Tweens::EaseIn);

		gunFeedbackRotate->StartDuration(gunFeedbackTimer, Function<void(gce::GameTransform::*)(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalRotation, &mp_playerGun->transform));
	}
}

//SCRAPPED
void PlayerController::useSecondary() {
	std::cout << "Secondary" << std::endl;
	// if no items :
	return;
	// else:
	//  Have something that checks which Secondary is equipped and do stuff based on that
}


void PlayerController::Update() {

	if (gce::GetKeyDown(gce::Keyboard::ESC))
	{
		ArenaManager::GetInstance()->SetState("Pause");
	}

	//CAMERA ROTATIONS
	{
		int width = gce::GameManager::GetWindow()->GetWidth();
		int height = gce::GameManager::GetWindow()->GetHeight();
		gce::Vector2i32 centerPos = gce::Vector2i32(width / 2, height / 2);
		gce::Vector2i32 currentPos = gce::GetMousePosition();

		gce::SetMousePosition(centerPos);

		float newX = (float)(currentPos.x - centerPos.x) / 300;
		float newY = (float)(currentPos.y - centerPos.y) / 300;

		const float yMax = 0.9f;
		const float yMin = -0.9f;

		// Prevent going out of y bounds and flipping over the camera
		newY = mp_playerCamera->transform.GetWorldForward().y - newY >= yMax ? gce::Max(mp_playerCamera->transform.GetWorldForward().y - yMax, newY) : newY;
		newY = mp_playerCamera->transform.GetWorldForward().y - newY <= yMin ? gce::Min(mp_playerCamera->transform.GetWorldForward().y - yMin, newY) : newY;

		mp_playerCamera->transform.LocalRotate(gce::Vector3f32(newY, 0, 0));
		mp_playerCamera->transform.WorldRotate(gce::Vector3f32(0, newX, 0));
	}

	//SHOOT
	if (gce::GetButtonDown(gce::Mouse::LEFT)) {
		Shoot();
	}
	//PICKUP
	if (gce::GetKeyDown(gce::Keyboard::E)) {
		ArenaManager::GetInstance()->GetItemManager()->ObtainItem();
	}
	//(Scrapped) SECONDARY
	if (gce::GetButtonDown(gce::Mouse::RIGHT)) {
		useSecondary();
	}

	//RELOAD
	if (gce::GetKeyDown(gce::Keyboard::R)) {
		if (not hasReload && not hasShot) {
			playerData->ammoLeft = playerData->ammoCapacity;
			std::cout << "Reload" << std::endl;

			gce::Tween<gce::Vector3f32>* reloadFeedback = nullptr;
			gce::Tween<gce::Vector3f32>* reloadFeedbackRotate = nullptr;

			reloadFeedback = &gce::TweenSystem::Create<gce::Vector3f32>(mp_playerGun->transform.GetLocalPosition(),
				mp_playerGun->transform.GetLocalPosition() + gce::Vector3f32(0, -0.05, 0), gce::Tweens::EaseIn);

			reloadFeedback->StartDuration(reloadFeedbackTimer / 3, Function<void(gce::GameTransform::*)(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalPosition, &mp_playerGun->transform));
			hasReload = true;

			reloadFeedbackRotate = &gce::TweenSystem::Create<gce::Vector3f32>(gce::Vector3f32(0, 0, 0), gce::Vector3f32(0.2, 0, 0), gce::Tweens::EaseIn);

			reloadFeedbackRotate->StartDuration(reloadFeedbackTimer / 3, Function<void(gce::GameTransform::*)(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalRotation, &mp_playerGun->transform));
		}
	}

	//JUMP
	if (gce::GetKeyDown(gce::Keyboard::SPACE)) {

		if (isCrouching || isJumping)
		{
			return;
		}

		std::cout << "Jump" << std::endl;
		isJumping = true;
		verticalVelocity = 0.300f;
	}
	
	//FEEDBACK BOOLS
	if (hasShot) {
		currentGunFeedbackTimer += gce::GameManager::DeltaTime();

		if (currentGunFeedbackTimer >= gunFeedbackTimer) {
			gce::Tween<gce::Vector3f32>* gunFeedbackward = nullptr;
			gce::Tween<gce::Vector3f32>* gunFeedbackwardRotate = nullptr;

			gunFeedbackward = &gce::TweenSystem::Create<gce::Vector3f32>(mp_playerGun->transform.GetLocalPosition(),
				gce::Vector3f32(0.35f, -0.45f, 0.8f), gce::Tweens::EaseIn);

			gunFeedbackward->StartDuration(gunFeedbackTimer, Function<void(gce::GameTransform::*)(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalPosition, &mp_playerGun->transform));


			gunFeedbackwardRotate = &gce::TweenSystem::Create<gce::Vector3f32>(gce::Vector3f32(-0.2, 0, 0), gce::Vector3f32(0, 0, 0), gce::Tweens::EaseIn);

			gunFeedbackwardRotate->StartDuration(gunFeedbackTimer, Function<void(gce::GameTransform::*)(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalRotation, &mp_playerGun->transform));

			hasShot = false;
			currentGunFeedbackTimer = 0;
		}
	}

	if (hasReload) {
		currentReloadFeedbackTimer += gce::GameManager::DeltaTime();

		if (currentReloadFeedbackTimer >= reloadFeedbackTimer) {
			gce::Tween<gce::Vector3f32>* reloadFeedbackward = nullptr;
			gce::Tween<gce::Vector3f32>* reloadFeedbackwardRotate = nullptr;

			reloadFeedbackward = &gce::TweenSystem::Create<gce::Vector3f32>(mp_playerGun->transform.GetLocalPosition(),
				gce::Vector3f32(0.35f, -0.45f, 0.8f), gce::Tweens::EaseIn);

			reloadFeedbackward->StartDuration(reloadFeedbackTimer, Function<void(gce::GameTransform::*)(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalPosition, &mp_playerGun->transform));


			reloadFeedbackwardRotate = &gce::TweenSystem::Create<gce::Vector3f32>(gce::Vector3f32(0.2, 0, 0), gce::Vector3f32(0, 0, 0), gce::Tweens::EaseIn);
			
			reloadFeedbackwardRotate->StartDuration(reloadFeedbackTimer, Function<void(gce::GameTransform::*)(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalRotation, &mp_playerGun->transform));


			hasReload = false;
			currentReloadFeedbackTimer = 0;
		}
	}

	if (playerData->hasDamaged) {
		playerData->currentDamageFeedbackTimer += gce::GameManager::DeltaTime();

		if (playerData->currentDamageFeedbackTimer >= playerData->damageFeedbackTimer) {

			gce::Tween<gce::Vector3f32>* damageFeedbackward = nullptr;

			damageFeedbackward = &gce::TweenSystem::Create<gce::Vector3f32>(gce::Vector3f32(0, 0, -0.15) * mp_playerCamera->transform.GetLocalForward(), gce::Vector3f32(0, 0, 0), gce::Tweens::EaseIn);

			damageFeedbackward->StartDuration(playerData->damageFeedbackTimer, Function<void(gce::GameTransform::*)(gce::Vector3f32 const&)>(&gce::GameTransform::SetLocalRotation, &m_pOwner->transform));
			playerData->hasDamaged = false;
			playerData->currentDamageFeedbackTimer = 0;
		}
	}
}

void PlayerController::FixedUpdate() {

	//JUMP
	if (isJumping) {
		m_pOwner->transform.WorldTranslate(gce::Vector3f32(0, verticalVelocity, 0));
		verticalVelocity -= 0.010f;
		verticalVelocity = gce::Max(0, verticalVelocity);
	}
	//CROUCH
	else {
		//HOLD CROUCH
		if (gce::GetKey(gce::Keyboard::LCTRL)) {
			isCrouching = true;
			playerData->crouchSlowValue = (playerData->moveSpeed / 5) * 3;
			mp_playerCamera->transform.SetLocalPosition(gce::Vector3f32(0, m_cameraOffset + m_crouchOffset, 0));
		}
		//RELEASE CROUCH
		else if (gce::GetKeyUp(gce::Keyboard::LCTRL)) {
			isCrouching = false;
			mp_playerCamera->transform.SetLocalPosition(gce::Vector3f32(0, m_cameraOffset, 0));
		}
	}

	//RUN
	if (gce::GetKey(gce::Keyboard::LSHIFT)) {
		if (not isCrouching) {
			playerData->runSpeedValue = playerData->moveSpeed * 1.5;
		}
	}

	//RIGHT
	if (gce::GetKey(gce::Keyboard::D)) {
		m_pOwner->transform.LocalTranslate(mp_playerCamera->transform.GetLocalRight() * (playerData->moveSpeed - playerData->crouchSlowValue) * gce::GameManager::FixedDeltaTime());
	}
	//FORWARD
	if (gce::GetKey(gce::Keyboard::Z) or
		gce::GetKey(gce::Keyboard::W)) {
		gce::Vector3f32 forward = gce::Vector3f32(mp_playerCamera->transform.GetLocalForward().x, 0, mp_playerCamera->transform.GetLocalForward().z).Normalize();
		m_pOwner->transform.LocalTranslate(forward * (playerData->moveSpeed + playerData->runSpeedValue - playerData->crouchSlowValue) * gce::GameManager::FixedDeltaTime());
	}
	//LEFT
	if (gce::GetKey(gce::Keyboard::Q) or
		gce::GetKey(gce::Keyboard::A)) {
		m_pOwner->transform.LocalTranslate(mp_playerCamera->transform.GetLocalRight() * (-playerData->moveSpeed + playerData->crouchSlowValue) * gce::GameManager::FixedDeltaTime());
	}
	//BACKWARD
	if (gce::GetKey(gce::Keyboard::S)) {
		gce::Vector3f32 forward = gce::Vector3f32(mp_playerCamera->transform.GetLocalForward().x, 0, mp_playerCamera->transform.GetLocalForward().z).Normalize();
		m_pOwner->transform.LocalTranslate(forward * (-playerData->moveSpeed + playerData->crouchSlowValue) * gce::GameManager::FixedDeltaTime());
	}

	playerData->runSpeedValue = 0;
	playerData->crouchSlowValue = 0;
}


void PlayerController::CollisionEnter(gce::GameObject* other)
{
	if (other->GetName() == "Floor")
	{
		isJumping = false;
	}
}

