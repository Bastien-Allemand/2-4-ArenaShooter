#ifndef ENGINE_COMPONENTS_BUTTON_H_INCLUDED
#define ENGINE_COMPONENTS_BUTTON_H_INCLUDED

#include <Event.hpp>
#include <Structs.h>
#include "define.h"
#include "UiBase.h"


namespace gce {

struct RectanglePosF;
struct UiBrush;
struct BitMapBrush;
struct ShapeUi;
struct ColorBrush;


DECLARE_COMPONENT( UiButton, UiBase )
{
    enum Shape: uint8 { RECT, ROUND_RECT, ELLIPSE, LINE };

    BitMapBrush* pBitMapBrush = nullptr;
    BitMapBrush* pHoverBitMapBrush = nullptr;

    bool disabled = false;

    void AddListener(Event<>::function pListener);

private:
    void Init() override;
    void UpdateRect( uint8 dirty );

    bool m_hover = false;
    bool m_pressed = false;
    
    BitMapBrush** m_ppActiveBrush = &pBitMapBrush;
    RectanglePosF m_rect { 0.0f, 0.0f, 0.0f, 0.0f };
    Event<> m_onClickEvent;

    friend class UiSystem;
};


}

#endif