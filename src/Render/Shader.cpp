#include "pch.h"
#include "Shader.h"

#include <filesystem>
#include <fstream>

namespace gce
{

	Shader::Shader(StringView const path)
	{
		Load(path);
	}

	Shader::Shader(Shader const& other)
	{
		m_size = other.m_size;
		m_pData = malloc(other.m_size);
		memcpy(m_pData, other.m_pData, other.m_size);
	}

	Shader::Shader(Shader&& other) noexcept
	{
		m_size = other.m_size;
		m_pData = other.m_pData;
		other.m_pData = nullptr;
	}

	Shader& Shader::operator=(Shader const& other)
	{
		m_size = other.m_size;
		m_pData = malloc(other.m_size);
		memcpy(m_pData, other.m_pData, other.m_size);
		return *this;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		m_size = other.m_size;
		m_pData = other.m_pData;
		other.m_pData = nullptr;
		return *this;
	}

	void Shader::Load(StringView const path)
	{
		std::ifstream vsFile(std::string(RES_PATH) + path.data(), std::ios::binary);
		if (vsFile.is_open())
		{
			vsFile.seekg(0, std::ios::end);
			m_size = vsFile.tellg();
			vsFile.seekg(0, std::ios::beg);
			m_pData = malloc(m_size);

			if (m_pData) vsFile.read(static_cast<char*>(m_pData), m_size);
		}
	}

	Shader::~Shader()
	{
		free(m_pData);
	}
	
}
