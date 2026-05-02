#ifndef ENGINE_COMPONENTS_H_INCLUDED
#define ENGINE_COMPONENTS_H_INCLUDED


#include "Engine/Components/Base.h"

// Camera
#include "Engine/Components/Camera.h"


/* ---- UI ---- */

// Button
#include "Engine/Components/UiButton.h"
#include "Engine/Components/UiImage.h"
#include "Engine/Components/TextRenderer.h"

/* ----- 2D ----- */

// Rendering
#include "Engine/Components/SpriteRenderer.h"
#include "Engine/Components/Animator2D.h"

// Colliders
#include "Engine/Components/ColliderBase2D.h"
#include "Engine/Components/BoxCollider2D.h"
#include "Engine/Components/CircleCollider2D.h"

/* ----- 3D ----- */

// Rendering
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/SkyBoxComponent.h"
#include "Engine/Components/Light.h"

// Colliders
#include "Engine/Components/SphereCollider.h"
#include "Engine/Components/BoxCollider.h"

//Physics
#include "Engine/Components/PhysicComponent.h"



#endif