#ifndef ENGINE_UI_SYSTEM_H_INCLUDED
#define ENGINE_UI_SYSTEM_H_INCLUDED

#include "define.h"

namespace gce {


class UiSystem final
{
	UiSystem() = default;
	~UiSystem() = default;

	// ===========================================
	/// @brief Operates every UI Component.
	/// @note Operate order is the following :
	/// @note Button
	// ===========================================
	void HandleUiUpdate();
	
	// ========================================
	/// @brief Draws every UI Component.
	/// @note Draw order is the following :
	/// @note Button -> TextRenderer
	// ========================================
	void HandleUiRendering();

	friend class GameManager;
};


}

#endif