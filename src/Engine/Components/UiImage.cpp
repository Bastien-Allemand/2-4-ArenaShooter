#include "pch.h"
#include "UiImage.h"

namespace gce
{
    void UiImage::InitializeImage(Vector2f32 const& pos, Vector2f32 const& size, float32 const opacity)
    {
        this->size = size;
        position = pos;
        rectImage = new RectanglePosF(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    }
} // namespace gce