#include "pch.h"
#include "RenderSystem.h"

#include <FrameResources.h>
#include <LightManager.h>
#include <RenderWindow.h>
#include <SplitScreenWindow.h>

#include "Components/MeshRenderer.h"
#include "Components/Animator2D.h"
#include "Components/SkyBoxComponent.h"
#include "GameObject.h"
#include "GameManager.h"

namespace gce {

// Animator2D
// Camera
// Skybox
// MeshRenderer


void RenderSystem::Handle3DRendering()
{
    WindowParam const& windowParam = GameManager::GetWindowParam();
    Window* const pWindow = GameManager::GetWindow();

    FrameResourcesManager::BeginFrame();

    FrameResourcesManager::UpdateObjCBuffer();

    LightManager::UpdateLightCB();

    pWindow->Begin3D();

    // Animator2D Logic TODO CHANGE: Animator2D shouldn't be using MeshRenderer but rather SpriteRenderer
    for ( uint16 i = 0; i < Animator2D::s_list.Size(); i++ )
    {
        Animator2D& animator = *Animator2D::s_list[i];
        if ( animator.m_created == false ) continue;
        if ( animator.IsActive() == false ) continue;

        if ( animator.m_textures.Empty() || animator.m_isPlaying == false ) continue;

        animator.m_timer += GameManager::DeltaTime();
        MeshRenderer& meshRenderer = *animator.m_pOwner->GetComponent<MeshRenderer>();

        if ( animator.m_timer < animator.m_timeBetweenFrames )
        {
            if ( meshRenderer.pMaterial->useTextureAlbedo )
                meshRenderer.pMaterial->albedoTextureID = animator.m_textures[animator.m_currentTexture]->GetTextureID();
            continue;
        }

        meshRenderer.pMaterial->albedoTextureID = animator.m_textures[animator.m_currentTexture]->GetTextureID();

        animator.m_currentTexture++;

        if ( animator.m_looping && animator.m_currentTexture >= animator.m_textures.Size() - 1 )
        {
            animator.m_isPlaying = false;
            return;
        }

        if ( animator.m_currentTexture >= animator.m_textures.Size() )
            animator.m_currentTexture = 0;

        animator.m_timer -= animator.m_timeBetweenFrames;
    }

    // Camera Logic
    for ( Camera* const pCamera : GameManager::GetMainCameras() )
    {
        if ( pCamera->m_active == false || pCamera->m_created == false ) continue;
        if ( windowParam.isSplitScreen ) pWindow->SetScreenToDraw( pCamera->screenId );
        pWindow->SetCamera( *pCamera );

        // Skybox Drawing
        for (uint16 i = 0; i < SkyBoxComponent::s_list.Size(); i++) // Liste de Skybox
        {
            SkyBoxComponent& skyboxComponent = *SkyBoxComponent::s_list[i];
            if (skyboxComponent.m_created == false) continue;
            if (skyboxComponent.IsActive() == false) continue;

            pWindow->GetRenderTarget()->DrawSkyBox(skyboxComponent);
        }

        // MeshRenderer Logic
        for ( uint16 i = 0; i < MeshRenderer::s_list.Size(); i++ )
        {
            MeshRenderer& meshRenderer = *MeshRenderer::s_list[i];
            if ( meshRenderer.m_created == false ) continue;
            if ( meshRenderer.IsActive() == false ) continue;

            pWindow->Draw( meshRenderer );
        }
    }

    pWindow->EndDraw();

    FrameResourcesManager::EndFrame();
}

}