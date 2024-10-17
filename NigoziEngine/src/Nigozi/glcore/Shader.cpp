#include "ngpch.h"

#include "Shader.h"
#include "DebugNMacros.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace Nigozi
{
    Shader::Shader(const std::string& filePath)
    {
        m_filePath = filePath;
        ShaderProgramSource shaderSource = ParseShader();
        m_shader = CreateShader(shaderSource.VertexShader, shaderSource.FragmentShader);
        GLCall(glUseProgram(m_shader));
    }

    Shader::~Shader()
    {
        Delete();
    }

    void Shader::SetUniform1i(const std::string& name, int value)
    {
        GLCall(glUniform1i(GetUniformLocation(name), value));
    }

    void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
    {
        GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
    }

    void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
    {
        GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
    }

    void Shader::Bind() const
    {
        GLCall(glUseProgram(m_shader));
    }

    void Shader::Unbind() const
    {
        GLCall(glUseProgram(0));
    }

    ShaderProgramSource Shader::ParseShader()
    {
        std::ifstream stream(m_filePath);
        std::stringstream ss[2];

        enum class ShaderType {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        ShaderType type = ShaderType::NONE;

        std::string line;
        while (std::getline(stream, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos) {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos) {
                    type = ShaderType::FRAGMENT;
                }
            }
            else {
                ss[(int)type] << line << "\n";
            }
        }

        return ShaderProgramSource{ ss[0].str(), ss[1].str() };
    }

    unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        GLCall(glShaderSource(id, 1, &src, nullptr));
        GLCall(glCompileShader(id));

        return id;
    }

    unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        unsigned int program = glCreateProgram();
        unsigned int _vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int _fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        GLCall(glAttachShader(program, _vertexShader));
        GLCall(glAttachShader(program, _fragmentShader));
        GLCall(glLinkProgram(program));
        GLCall(glValidateProgram(program));
        GLCall(glDeleteShader(_vertexShader));
        GLCall(glDeleteShader(_fragmentShader));

        return program;
    }

    int Shader::GetUniformLocation(const std::string& uniform)
    {
        return GLCall(glGetUniformLocation(m_shader, uniform.c_str()));
    }

    void Shader::Delete()
    {
        if (m_shader != -2) {
            GLCall(glDeleteProgram(m_shader));
            m_shader = -2;
        }
    }
}
