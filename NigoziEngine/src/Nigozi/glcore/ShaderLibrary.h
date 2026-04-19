#pragma once

#include "ngpch.h"
#include "Shader.h"
#include "ref/Ref.h"

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

		const Ref<Shader> AddShader(const std::string& filePath);
		bool DeleteShader(const std::string& name);

		const Ref<Shader> GetShader(const std::string& name) const { return m_shaderMap.at(name); }
		const std::map<std::string, Ref<Shader>>& GetShaderMap() const { return m_shaderMap; }
	private:
		// Shaders will be stored in a map for fast access
		std::map<std::string, Ref<Shader>> m_shaderMap;
	};
}

