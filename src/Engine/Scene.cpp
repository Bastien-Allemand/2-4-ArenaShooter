#include "pch.h"
#include "Scene.h"

#include <ranges>

#include "GameObject.h"
#include "GameManager.h"

namespace gce {


Scene& Scene::Create()
{
    Scene* const pNew = new Scene();
    GameManager::s_pInstance->m_scenes.PushBack( pNew );
    return *pNew;
}

GameObject* Scene::GetGameObject(uint32 id) const
{
    if (m_gameObjects.contains(id) == false)
        return nullptr;
    
    return m_gameObjects.at(id);
}

void Scene::UpdateMatrix()
{
    for (GameObject* const pGameObject : m_gameObjects | std::views::values)
    {
        if (pGameObject->IsActive() == false) continue;

        pGameObject->transform.UpdateMatrix();
    }
}

}