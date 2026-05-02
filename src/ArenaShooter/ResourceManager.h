#pragma once
#include <Engine.h>

class ResourceManager
{
	std::unordered_map<String, gce::Texture*> m_textures;
	std::unordered_map<std::wstring, gce::Font*> m_fonts;
	std::unordered_map<std::string_view, gce::Geometry*> m_geometries;
	std::unordered_map<std::string, gce::BitMapBrush*> m_bitmapBrush;
	void Init();

public:
	static gce::Texture* GetTexture(String _name);
	static gce::Font* GetFont(std::wstring _name);
	static gce::Geometry* GetGeometry(std::string_view _name);
	static gce::BitMapBrush* GetBitMapBrush(std::string _name);

	friend class ArenaManager;
};

