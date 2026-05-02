#include "pch.h"
#include "LifespanSystem.h"
#include "GameManager.h"
#include "ScriptSystem.h"
#include "Script.h"

#include <FrameResources.h>

#include "Components.h"
#include "GameObject.h"
#include "Scene.h"
#include "Components/Light.h"

namespace gce {


void LifespanSystem::HandleCreations()
{
    while ( m_toCreate.gameObjects.Empty() == false )
    {
        GameObject* pGameObject = m_toCreate.gameObjects.Front();
        m_toCreate.gameObjects.Pop();
        pGameObject->m_pScene->m_gameObjects[pGameObject->m_id] = pGameObject;
        pGameObject->m_created = true;
        pGameObject->transform.UpdateMatrix();
    }

    while ( Camera::s_creationList.Empty() == false )
    {
        Camera& camera = *Camera::s_list[Camera::s_creationList.Front()];
        Camera::s_creationList.Pop();
        camera.m_created = true;
        if ( camera.GetType() == PERSPECTIVE )
            camera.perspective.aspectRatio = GameManager::GetWindow()->GetAspectRatio();
    }
    
    while ( Light::s_creationList.Empty() == false )
    {
        Light& light = *Light::s_list[Light::s_creationList.Front()];
        Light::s_creationList.Pop();
        light.m_created = true;
        LightManager::AddLight(light);
    }

    while (  MeshRenderer::s_creationList.Empty() == false  )
    {
        MeshRenderer& meshRenderer = *MeshRenderer::s_list[MeshRenderer::s_creationList.Front()];
        MeshRenderer::s_creationList.Pop();
        meshRenderer.m_created = true;
        FrameResourcesManager::RemoveRenderItem( reinterpret_cast<RenderItem&>(meshRenderer) );
    }

    while (TextRenderer::s_creationList.Empty() == false)
    {
        TextRenderer& text = *TextRenderer::s_list[TextRenderer::s_creationList.Front()];
        TextRenderer::s_creationList.Pop();
        text.m_created = true;
    }

    while (Animator2D::s_creationList.Empty() == false)
    {
        Animator2D& animator2D = *Animator2D::s_list[Animator2D::s_creationList.Front()];
        Animator2D::s_creationList.Pop();
        animator2D.m_created = true;
    }

    while( SkyBoxComponent::s_creationList.Empty() == false )
    {
        SkyBoxComponent& skyBoxComponent = *SkyBoxComponent::s_list[SkyBoxComponent::s_creationList.Front()];
        SkyBoxComponent::s_creationList.Pop();
        skyBoxComponent.m_created = true;
    }

    while ( SphereCollider::s_creationList.Empty() == false )
    {
        SphereCollider& sphereCollider = *SphereCollider::s_list[SphereCollider::s_creationList.Front()];
        SphereCollider::s_creationList.Pop();
        sphereCollider.m_created = true;
    }

    while ( BoxCollider::s_creationList.Empty() == false )
    {
        BoxCollider& boxCollider = *BoxCollider::s_list[BoxCollider::s_creationList.Front()];
        BoxCollider::s_creationList.Pop();
        boxCollider.m_created = true;
    }
    
    while (CircleCollider2D::s_creationList.Empty() == false)
    {
        CircleCollider2D& circleCollider = *CircleCollider2D::s_list[CircleCollider2D::s_creationList.Front()];
        CircleCollider2D::s_creationList.Pop();
        circleCollider.m_created = true;
    }

    while (BoxCollider2D::s_creationList.Empty() == false)
    {
        BoxCollider2D& boxCollider = *BoxCollider2D::s_list[BoxCollider2D::s_creationList.Front()];
        BoxCollider2D::s_creationList.Pop();
        boxCollider.m_created = true;
    }

    while ( PhysicComponent::s_creationList.Empty() == false )
    {
        PhysicComponent& physicComponent = *PhysicComponent::s_list[PhysicComponent::s_creationList.Front()];
        PhysicComponent::s_creationList.Pop();
        physicComponent.m_created = true;
    }
    
    while (UiButton::s_creationList.Empty() == false)
    {
        UiButton& buttonComponent = *UiButton::s_list[UiButton::s_creationList.Front()];
        UiButton::s_creationList.Pop();
        buttonComponent.m_created = true;
    }

    while (UiImage::s_creationList.Empty() == false)
    {
        UiImage& UiImageComponent = *UiImage::s_list[UiImage::s_creationList.Front()];
        UiImage::s_creationList.Pop();
        UiImageComponent.m_created = true;
    }

    // Always last
    while (Script::s_creationList.Empty() == false)
    {
        Script* pScript = Script::s_list[Script::s_creationList.Front()];
        pScript->Init(); // Creation equivalent to m_created = true;
        Script::s_creationList.Pop();
    }
    GameManager::GetScriptSystem().StartScripts();
}

void LifespanSystem::HandleDeletions()
{
    // Always first
    while (Script::s_deletionList.Empty() == false)
    {
        Script* pScript = Script::s_list[Script::s_deletionList.Front()];
        pScript->UnInit();
        Script::s_deletionList.Pop();
        Script::s_idsList.Push(pScript->m_id);
        delete pScript;
    }

    while (UiButton::s_deletionList.Empty() == false)
    {
        UiButton& buttonComponent = *UiButton::s_list[UiButton::s_deletionList.Front()];
        UiButton::s_deletionList.Pop();
        buttonComponent.m_created = false;
        buttonComponent.m_destroyed = false;
        UiButton::s_idsList.Push(buttonComponent.m_id);
    }

    while (UiImage::s_deletionList.Empty() == false)
    {
        UiImage& UiImageComponent = *UiImage::s_list[UiImage::s_deletionList.Front()];
        UiImage::s_deletionList.Pop();
        UiImageComponent.m_created = false;
        UiImageComponent.m_destroyed = false;
        UiImage::s_idsList.Push(UiImageComponent.m_id);
    }

    while (PhysicComponent::s_deletionList.Empty() == false)
    {
        PhysicComponent& physicComponent = *PhysicComponent::s_list[PhysicComponent::s_deletionList.Front()];
        PhysicComponent::s_deletionList.Pop();
        physicComponent.m_created = false;
        physicComponent.m_destroyed = false;
        PhysicComponent::s_idsList.Push(physicComponent.m_id);
    }

    while (BoxCollider::s_deletionList.Empty() == false)
    {
        BoxCollider& boxCollider = *BoxCollider::s_list[BoxCollider::s_deletionList.Front()];
        BoxCollider::s_deletionList.Pop();
        boxCollider.m_created = false;
        boxCollider.m_destroyed = false;
        BoxCollider::s_idsList.Push(boxCollider.m_id);
    }

    while (BoxCollider2D::s_deletionList.Empty() == false)
    {
        BoxCollider2D& boxCollider = *BoxCollider2D::s_list[BoxCollider2D::s_deletionList.Front()];
        BoxCollider2D::s_deletionList.Pop();
        boxCollider.m_created = false;
        boxCollider.m_destroyed = false;
        BoxCollider2D::s_idsList.Push(boxCollider.m_id);
    }

    while (SphereCollider::s_deletionList.Empty() == false)
    {
        SphereCollider& sphereCollider = *SphereCollider::s_list[SphereCollider::s_deletionList.Front()];
        SphereCollider::s_deletionList.Pop();
        sphereCollider.m_created = false;
        sphereCollider.m_destroyed = false;
        SphereCollider::s_idsList.Push(sphereCollider.m_id);
    }

	while (CircleCollider2D::s_deletionList.Empty() == false)
	{
		CircleCollider2D& circleCollider = *CircleCollider2D::s_list[CircleCollider2D::s_deletionList.Front()];
		CircleCollider2D::s_deletionList.Pop();
		circleCollider.m_created = false;
		circleCollider.m_destroyed = false;
		CircleCollider2D::s_idsList.Push(circleCollider.m_id);
	}

	while (Animator2D::s_deletionList.Empty() == false)
	{
		Animator2D& animator2D = *Animator2D::s_list[Animator2D::s_deletionList.Front()];
		Animator2D::s_deletionList.Pop();
		animator2D.m_created = false;
		animator2D.m_destroyed = false;
		Animator2D::s_idsList.Push(animator2D.m_id);
	}

    while ( MeshRenderer::s_deletionList.Empty() == false )
    {
        MeshRenderer& meshRenderer = *MeshRenderer::s_list[MeshRenderer::s_deletionList.Front()];
        MeshRenderer::s_deletionList.Pop();
        meshRenderer.m_created = false;
        meshRenderer.m_destroyed = false;
        MeshRenderer::s_idsList.Push( meshRenderer.m_id );
    }

    while (TextRenderer::s_deletionList.Empty() == false)
    {
        TextRenderer& text = *TextRenderer::s_list[TextRenderer::s_deletionList.Front()];
        TextRenderer::s_deletionList.Pop();
        text.m_created = false;
        text.m_destroyed = false;
        TextRenderer::s_idsList.Push(text.m_id);
    }
    
    while (SkyBoxComponent::s_creationList.Empty() == false)
    {
        SkyBoxComponent& skyBoxComponent = *SkyBoxComponent::s_list[SkyBoxComponent::s_deletionList.Front()];
        SkyBoxComponent::s_deletionList.Pop();
        skyBoxComponent.m_created = false;
        skyBoxComponent.m_destroyed = false;
        SkyBoxComponent::s_idsList.Push( skyBoxComponent.m_id );
    }

    while ( Camera::s_deletionList.Empty() == false )
    {
        Camera& camera = *Camera::s_list[Camera::s_deletionList.Front()];
        Camera::s_deletionList.Pop();
        camera.m_created = false;
        camera.m_destroyed = false;
        Camera::s_idsList.Push( camera.m_id );
    }

    while ( Light::s_deletionList.Empty() == false )
    {
        Light& light = *Light::s_list[Light::s_deletionList.Front()];
        Light::s_deletionList.Pop();
        light.m_created = false;
        light.m_destroyed = false;
        Light::s_idsList.Push( light.m_id );
    }
    
    while ( m_toDelete.gameObjects.Empty() == false )
    {
        GameObject* const pGameObject = m_toDelete.gameObjects.Front();
        m_toDelete.gameObjects.Pop();
        pGameObject->m_pScene->m_gameObjects.erase( pGameObject->m_id );
        //for (int i = pGameObject->m_id + 1; i < pGameObject->m_pScene->m_gameObjects.size(); i++)
        //    pGameObject->m_pScene->m_gameObjects[i]->m_id--;
        delete pGameObject;
    }
}

}
