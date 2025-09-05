#pragma once

#include "ngpch.h"

namespace Nigozi
{
	struct ShaderProgramSource {
		std::string VertexShader;
		std::string FragmentShader;
	};

	// TODO
	// Create a shader library

	class Shader
	{
	public:
		Shader() = default;
		Shader(const std::string& filePath);
		~Shader();

		void SetUniform1i(const std::string& name, int value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
		void SetUniform1iv(const std::string& name, uint32_t count, int* value);

		void Bind() const;
		void Unbind() const;

		uint32_t GetProgram() const { return m_shader; }
		const std::string& GetFilePath() const { return m_filePath; }

		int GetUniformLocation(const std::string& uniform);

		void Delete();
	private:
		ShaderProgramSource ParseShader();
		uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		uint32_t CompileShader(uint32_t type, const std::string& source);
	private:
		std::string m_filePath;
		uint32_t m_shader;
	};
}
