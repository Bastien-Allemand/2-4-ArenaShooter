#pragma once
#include "GameActor.h"
#include "Graph.hpp"
#include "Tile.h"

DECLARE_CHILD_SCRIPT(Enemy, GameActor, ScriptFlag::FixedUpdate | ScriptFlag::Start | ScriptFlag::CollisionEnter)

gce::StateMachine* states;

Node<Tile>* mp_currentNode;
Node<Tile>* mp_nextNode;
Path<Tile>* mp_path;
bool redirecting = false;

bool isDying = false;
float deathFeedbackTimer = 0.5f;
float currentDeathFeedbackTimer = 0;

void GoToPoint(gce::Vector3f32 point); // Pathfinding implementation

void FindPathToPlayer();
void CollisionEnter(gce::GameObject* other);
void Start();
virtual void Update() override;
virtual void Death() override;

END_SCRIPT

