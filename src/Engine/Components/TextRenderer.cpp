#include "pch.h"
#include "TextRenderer.h"
#include <Font.h>
#include <Structs.h>
#include <FrameResources.h>

namespace gce {
	void TextRenderer::Init()
	{
		pFont = new Font(L"Arial");
		pBrush = new ColorBrush(Color::White);
		text = L"Hello, World!";
		rectPosF = new RectanglePosF(0.0f, 0.0f, 200.0f, 50.0f);
		scale = 1.f;
	}

	void TextRenderer::SetCenter(gce::Vector2f32 center, gce::Vector2f32 size)
	{
		rectPosF->top = center.y - size.y;
		rectPosF->bottom = center.y + size.y;
		rectPosF->left = center.x - size.x;
		rectPosF->right = center.x + size.x;
	}

} // namespace gce