#include "ngpch.h"
#include "ShaderLibrary.h"

namespace Nigozi 
{
	ShaderLibrary::ShaderLibrary()
	{
	}

	ShaderLibrary::~ShaderLibrary()
	{
	}

	const std::shared_ptr<Shader>& ShaderLibrary::AddShader(const std::string& filePath)
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(filePath);

		size_t slash_pos = filePath.find_last_of("/\\");
		std::string filename = (slash_pos == std::string::npos) ? filePath : filePath.substr(slash_pos + 1);

		size_t dot_pos = filename.find_last_of('.');
		std::string filename_no_ext = (dot_pos == std::string::npos) ? filename : filename.substr(0, dot_pos);

		m_shaderMap[filename_no_ext] = shader;

		return shader;
	}

	bool ShaderLibrary::DeleteShader(const std::string& name)
	{
		if (m_shaderMap.at(name)) {
			m_shaderMap.erase(name);
			return true;
		}
		return false;
	}
}
