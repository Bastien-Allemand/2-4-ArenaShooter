#include "pch.h"
#include "Font.h"
#include "RenderContext.h"

namespace gce
{

	Font::Font(std::wstring_view name)
	{
		Load(name);
	}

	Font::Font(Font const& other)
	{
		m_pFormat = other.m_pFormat;
		m_pFormat->AddRef();
	}

	Font& Font::operator=(Font const& other)
	{
		m_pFormat = other.m_pFormat;
		m_pFormat->AddRef();
		return *this;
	}

	Font::~Font()
	{
		m_pFormat->Release();
	}

	void Font::Load(std::wstring_view const& name)
	{
		ThrowIfFailed(RenderContext::GetWriteFactory()->CreateTextFormat(
			name.data(),
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			50,
			L"en-us",
			&m_pFormat
		), "Failed to create text format");
	}
}
