#include "ItemDropBase.h"
#include "ArenaManager.h"
#include <Engine.h>

ItemDropBase::ItemDropBase()
{
	mp_itemSphere = &gce::GameObject::Create(*(ArenaManager::GetInstance()->GetScene()));
}

void ItemDropBase::InitGeometry()
{
	gce::MeshRenderer* meshRenderer = mp_itemSphere->AddComponent<gce::MeshRenderer>();

	meshRenderer->SetGeometry(ResourceManager::GetGeometry("res/Models/Items/" + std::to_string(data->GetID()) + ".obj"));
	meshRenderer->SetAlbedoTexture(ResourceManager::GetTexture("res/Textures/Items/" + std::to_string(data->GetID()) + ".png"));
}

ItemDropBase::~ItemDropBase()
{
	mp_itemSphere->Destroy();
}
