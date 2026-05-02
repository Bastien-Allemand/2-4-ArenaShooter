#include <Engine.h>
#include "define.h"
#include "Bullet.h"
#include "ArenaManager.h"
#include "Player.h"

DECLARE_SCRIPT(PlayerController, ScriptFlag::FixedUpdate | ScriptFlag::Update | ScriptFlag::Awake | ScriptFlag::Start | ScriptFlag::CollisionEnter)

Player* playerData;

bool isCrouching = false;
bool isJumping = false;

bool hasShot = false;
float gunFeedbackTimer;
float currentGunFeedbackTimer = 0;

bool hasReload = false;
float reloadFeedbackTimer = 0.5f;
float currentReloadFeedbackTimer = 0;

const float m_cameraOffset = 1.8f;
const float m_crouchOffset = -0.8f;

gce::GameObject* mp_playerCamera;
gce::GameObject* mp_playerGun;

float verticalVelocity = 0;

void Shoot();
void useSecondary();

void InitializeCam();

void Awake();
void Start();
void Update();
void FixedUpdate();
void CollisionEnter(gce::GameObject* other);

END_SCRIPT