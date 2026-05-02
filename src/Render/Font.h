#pragma once

#include "define.h"

namespace gce
{

	class Font
	{
	public:
		Font() = default;
		Font(WStringView name);
		Font(Font const& other);
		Font(Font&& other) = delete;

		Font& operator=(Font const&);
		Font& operator=(Font&&) = delete;

		~Font();

		void Load(WStringView const& name);

	private:
		IDWriteTextFormat* m_pFormat = nullptr;
		ID2D1SolidColorBrush* m_pBrush = nullptr;

		friend class Window;
	};
}

