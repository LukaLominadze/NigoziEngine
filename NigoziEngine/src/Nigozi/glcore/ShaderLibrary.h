#pragma once

#include "ngpch.h"
#include "Shader.h"

namespace Nigozi
{
	/// <summary>
	/// This is where all the shaders will be located
	/// The library can be accessed from the Renderer
	/// and will be used for material systems.
	/// </summary>
	class ShaderLibrary
	{
	public:
		ShaderLibrary();
		~ShaderLibrary();

		const std::shared_ptr<Shader>& AddShader(const std::string& filePath);
		bool DeleteShader(const std::string& name);

		const std::shared_ptr<Shader>& GetShader(const std::string& name) const { return m_shaderMap.at(name); }
		const std::map<std::string, std::shared_ptr<Shader>>& GetShaderMap() const { return m_shaderMap; }
	private:
		// Shaders will be stored in a map for fast access
		std::map<std::string, std::shared_ptr<Shader>> m_shaderMap;
	};
}

