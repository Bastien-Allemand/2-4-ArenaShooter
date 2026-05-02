#pragma once
#include <Engine.h>
#include "Graph.hpp"
#include "Tile.h"

gce::GameObject& CreateImage(gce::BitMapBrush* btm, gce::Vector2f32 pos, gce::Vector2f32 scale, gce::Vector2f32 origin = { 0.5f, 0.5f }, float rotation = 0);

Node<Tile>* GetClosestNode(gce::Vector3f32 pos, Graph<Tile>* graph);