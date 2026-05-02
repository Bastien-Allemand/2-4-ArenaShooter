#include "ResourceManager.h"
#include "ArenaManager.h"

void ResourceManager::Init()
{

}

gce::Texture* ResourceManager::GetTexture(String _name)
{
	ResourceManager* resourceManager = ArenaManager::GetResourceManager();
	if (resourceManager->m_textures.contains(_name) == false)
	{
		resourceManager->m_textures[_name] = new gce::Texture(_name);
	}

	return resourceManager->m_textures[_name];
}

gce::Font* ResourceManager::GetFont(std::wstring _name)
{
	ResourceManager* resourceManager = ArenaManager::GetResourceManager();
	if (resourceManager->m_fonts.contains(_name) == false)
	{
		resourceManager->m_fonts[_name] = new gce::Font(_name);
	}

	return resourceManager->m_fonts[_name];
}

gce::Geometry* ResourceManager::GetGeometry(std::string_view _name)
{
	ResourceManager* resourceManager = ArenaManager::GetResourceManager();
	if (resourceManager->m_geometries.contains(_name) == false)
	{
		resourceManager->m_geometries[_name] = gce::GeometryFactory::LoadGeometry(_name);
	}

	return resourceManager->m_geometries[_name];
}
gce::BitMapBrush* ResourceManager::GetBitMapBrush(std::string _name)
{
	ResourceManager* resourceManager = ArenaManager::GetResourceManager();
	if (resourceManager->m_bitmapBrush.contains(_name) == false)
	{
		resourceManager->m_bitmapBrush[_name] = new gce::BitMapBrush(_name);
	}

	return resourceManager->m_bitmapBrush[_name];
}
