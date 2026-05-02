#ifndef ENGINE_TEXT_RENDERER_H_INCLUDED
#define ENGINE_TEXT_RENDERER_H_INCLUDED

#include "define.h"
#include "UIBase.h"
#include <Structs.h>
#include <Font.h>

namespace gce {

DECLARE_COMPONENT( TextRenderer, UiBase )
{
	std::wstring text;
	Font* pFont;
	UiBrush* pBrush;
	RectanglePosF* rectPosF;
	float scale;

protected:
	void Init() override;

public:
	void SetCenter(gce::Vector2f32 center, gce::Vector2f32 size);

	friend class UiSystem;
};


}

#endif