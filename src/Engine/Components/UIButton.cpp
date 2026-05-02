#include "pch.h"
#include "UiButton.h"

#include <InputsMethods.h>

#include "GameObject.h"

namespace gce {


void UiButton::Init()
{
    m_pOwner->transform.dirtyEvent.AddListener( &UiButton::UpdateRect, this );
}

void UiButton::UpdateRect( uint8 )
{
    Vector3f32 const& position = m_pOwner->transform.GetWorldPosition();
    Vector3f32 const& scale = m_pOwner->transform.GetWorldScale();

    m_rect.left   = position.x - 0.5f * scale.x;
    m_rect.top    = position.y - 0.5f * scale.y;
    m_rect.right  = position.x + 0.5f * scale.x;
    m_rect.bottom = position.y + 0.5f * scale.y;

    if ( pBitMapBrush )
    {
        Vector3f32 const brushScale = scale / Vector3f32{ pBitMapBrush->GetSize().x, pBitMapBrush->GetSize().y, 1.0f };
        pBitMapBrush->SetTransformMatrix( { m_rect.left, m_rect.top, 0.0f }, brushScale, 0.0f );
    }

    if ( pHoverBitMapBrush )
    {
        Vector3f32 const brushScale = scale / Vector3f32{ pHoverBitMapBrush->GetSize().x, pHoverBitMapBrush->GetSize().y, 1.0f };
        pHoverBitMapBrush->SetTransformMatrix( { m_rect.left, m_rect.top, 0.0f }, brushScale, 0.0f );
    }
}






    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Adds a listener to the onClick event.
    /// @param Takes a function pointer of a "void MyFunction()" type function.
    ///////////////////////////////////////////////////////////////////////////////
    void UiButton::AddListener(Event<>::function pListener)
    {
        m_onClickEvent.AddListener(pListener);
    }

}