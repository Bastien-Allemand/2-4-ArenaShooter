#include "pch.h"
#include "UiSystem.h"

#include "InputsMethods.h"

#include "Components/TextRenderer.h"
#include "Components/UiButton.h"
#include "GameObject.h"

#include "GameManager.h"

namespace gce {


void UiSystem::HandleUiUpdate()
{
	// Button
	for ( UiButton* const pButton : UiButton::s_list )
	{
		if ( pButton->IsActive() == false || pButton->m_created == false || pButton->disabled ) continue;

		Vector2i32 const& mousePosition = GetMousePosition();

		if ( mousePosition.x >= pButton->m_rect.left && mousePosition.x <= pButton->m_rect.right &&
				mousePosition.y >= pButton->m_rect.top && mousePosition.y <= pButton->m_rect.bottom )
		{
			if ( pButton->m_hover == false )
			{
				pButton->m_hover = true;
				pButton->m_ppActiveBrush = &pButton->pHoverBitMapBrush;
			}

			if ( pButton->m_pressed == false )
			{
				if ( GetButtonDown( Mouse::LEFT ) ) pButton->m_pressed = true;
				continue;
			}

			if ( GetButtonUp( Mouse::LEFT ) )
			{
				pButton->m_onClickEvent.Invoke();
				pButton->m_pressed = false;
			}

			continue;
		}

		if ( pButton->m_hover )
		{
			pButton->m_hover = false;
			pButton->m_ppActiveBrush = &pButton->pBitMapBrush;
		}

		if ( pButton->m_pressed && GetButtonUp( Mouse::LEFT ) )
			pButton->m_pressed = false;
	}
}


void UiSystem::HandleUiRendering()
{
	Window* const pWindow = GameManager::GetWindow();
	pWindow->BeginUI();

	for (UiImage* pImage : UiImage::s_list)
	{
		if (pImage->m_destroyed == true || pImage->IsActive() == false || pImage->m_created == false )
			continue;

		GameManager::GetWindow()->SetActiveBrush(*pImage->btmBrush);
		GameManager::GetWindow()->DrawShapeUI(*pImage->rectImage, 1);
	}
	// Button
	for ( UiButton* const pButton : UiButton::s_list )
	{
		if (pButton->m_destroyed == true || pButton->IsActive() == false || pButton->m_created == false ) continue;

		pWindow->SetActiveBrush( **pButton->m_ppActiveBrush );
		pWindow->DrawShapeUI( pButton->m_rect, true );
	}
	// TextRenderer
	for ( TextRenderer* const pTextRenderer : TextRenderer::s_list )
	{
		if (pTextRenderer->m_destroyed == true || pTextRenderer->IsActive() == false || pTextRenderer->m_created == false ) continue;

		pWindow->SetActiveBrush( *pTextRenderer->pBrush );
		pWindow->DrawTextUI( pTextRenderer->text, *pTextRenderer->pFont, *pTextRenderer->rectPosF, gce::Vector2f32(1, 1) * pTextRenderer->scale);
	}



	pWindow->EndUI();
}


}
