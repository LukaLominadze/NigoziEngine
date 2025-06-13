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
		void SetUniform1iv(const std::string& name, unsigned int count, int* value);

		void Bind() const;
		void Unbind() const;

		unsigned int GetProgram() const { return m_shader; }
		const std::string& GetFilePath() const { return m_filePath; }

		int GetUniformLocation(const std::string& uniform);

		void Delete();
	private:
		ShaderProgramSource ParseShader();
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		unsigned int CompileShader(unsigned int type, const std::string& source);
	private:
		std::string m_filePath;
		unsigned int m_shader;
	};
}
