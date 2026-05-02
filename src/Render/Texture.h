#ifndef RENDER_TEXTURE_H_INCLUDED
#define RENDER_TEXTURE_H_INCLUDED

#include "StaticBuffer.h"

namespace gce
{
	struct ImageData;

	class Texture
	{
	public:
		Texture() = default;
		Texture(std::string_view path);
		Texture(Texture const& other) = delete;
		Texture(Texture&& other) = delete;

		Texture& operator=(Texture const& other) = delete;
		Texture& operator=(Texture&& other) = delete;

		virtual ~Texture() = default;

		virtual void Create(std::string_view path);
		virtual void Create3D(std::string_view path);

		void SetID();

		uint32 GetWidth() const { return m_width; }
		uint32 GetHeight() const { return m_height; }
		uint32 GetTextureID() const { return m_textureID; }
		StaticBuffer* GetBuffer() { return &m_textureBuffer; }

	protected:
		StaticBuffer m_textureBuffer = {};

		uint32 m_textureID = -1;
		uint32 m_width = 0;
		uint32 m_height = 0;
	};
}

#endif