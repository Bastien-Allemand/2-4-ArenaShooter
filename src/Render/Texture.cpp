#include "pch.h"

#include "Texture.h"
#include "ImageLoader.h"
#include "RenderContext.h"
#include "Window.h"

namespace gce
{
	static uint32 globalID = 0;

	Texture::Texture(std::string_view const path)
	{
		Create(path);
	}

	void Texture::SetID()
	{
		static uint32 globalID = 0;
		m_textureID = globalID;

		globalID++;
	}

	void Texture::Create(std::string_view const path)
	{
		
		ImageData imgData = ImageLoader::LoadTexture(std::string(RES_PATH) + path.data());
		m_width = imgData.width;
		m_height = imgData.height;

		SetID();

		m_textureBuffer.InitTexture(imgData);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = imgData.giPixelFormat;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		RenderContext::GetDevice()->CreateShaderResourceView(m_textureBuffer.m_pDefaultBuffer, &srvDesc, RenderContext::GetTextureHeapHandle());
		RenderContext::GetTextureHeapHandle().Offset(RenderContext::CbvSrvUavDescriptorSize);
	}

	void Texture::Create3D(std::string_view path)
	{
		SetID();

		std::filesystem::path texturePath = std::string(RES_PATH) + path.data();

		int32 tempWidth, tempHeight;

		RenderContext::LoadDDS(texturePath.wstring().c_str(), &m_textureBuffer.m_pDefaultBuffer, &m_textureBuffer.m_pUploadBuffer, tempWidth, tempHeight);

		m_width = tempWidth;
		m_height = tempHeight;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = 1;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;

		RenderContext::GetDevice()->CreateShaderResourceView(m_textureBuffer.m_pDefaultBuffer, &srvDesc, RenderContext::GetTextureHeapHandle());
		RenderContext::GetTextureHeapHandle().Offset(RenderContext::CbvSrvUavDescriptorSize);
	}
}

