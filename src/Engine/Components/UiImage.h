#ifndef ENGINE_COMPONENT_UI_IMAGE_H_INCLUDED
#define ENGINE_COMPONENT_UI_IMAGE_H_INCLUDED

#include "define.h"
#include "UiBase.h"
#include "Structs.h"

namespace gce
{
    
    DECLARE_COMPONENT( UiImage, UiBase )
    {
        RectanglePosF* rectImage;
        BitMapBrush* btmBrush;

        void InitializeImage(Vector2f32 const& position, Vector2f32 const& size, float32 opacity);

        friend class UiSystem;
    };

} // namespace gce

#endif