#ifndef RENDER_SHADER_H_INCLUDED
#define RENDER_SHADER_H_INCLUDED

#include "define.h"

namespace gce
{

	class Shader
	{
	public:
		Shader() = default;
		Shader(StringView const path);
		Shader(Shader const& other);
		Shader(Shader&& other) noexcept;

		Shader& operator=(Shader const& other);
		Shader& operator=(Shader&& other) noexcept;

		~Shader();

		void Load(StringView const path);

		uint64 GetSize() const { return m_size; }
		void* GetData() const { return m_pData; }

	private:
		uint64 m_size = 0;
		void* m_pData = nullptr;
	};
	
}
#endif