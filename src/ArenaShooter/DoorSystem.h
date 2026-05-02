#pragma once
#include "define.h"
#include <Engine.h>

DECLARE_SCRIPT(DoorSystemScript, ScriptFlag::FixedUpdate | ScriptFlag::Start)

//States
bool open = false;
bool opening = false;
bool closed = true;
bool closing = false;
bool Locked = false;

gce::GameObject* pPlayer;

gce::Vector3f32 closedPosition;
gce::Vector3f32 openPosition;
gce::Vector3f32 offset;
void Start();

void FixedUpdate();

END_SCRIPT
