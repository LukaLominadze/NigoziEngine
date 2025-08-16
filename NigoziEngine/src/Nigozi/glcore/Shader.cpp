#include "ngpch.h"

#include "Shader.h"

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
        LOG("Creating shader at " + filePath);
        LOG("SHADER VERTEX");
        LOG(shaderSource.VertexShader);
        LOG("SHADER FRAGMENT");
        LOG(shaderSource.FragmentShader);
        m_shader = CreateShader(shaderSource.VertexShader, shaderSource.FragmentShader);
        GLCall(glLinkProgram(m_shader));
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

    void Shader::SetUniform1iv(const std::string& name, uint32_t count, int* value)
    {
        GLCall(glUniform1iv(GetUniformLocation(name), count, value));
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
            if (line.find("//type") != std::string::npos) {
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

    uint32_t Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        uint32_t program = glCreateProgram();
        uint32_t _vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
        uint32_t _fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        GLCall(glAttachShader(program, _vertexShader));
        GLCall(glAttachShader(program, _fragmentShader));
        GLCall(glLinkProgram(program));

        int success;
        GLCall(glGetProgramiv(program, GL_LINK_STATUS, &success));
        if (!success) {
            char infoLog[1024];
            GLCall(glGetProgramInfoLog(program, 1024, nullptr, infoLog));
            LOG("[Shader Linking Error]:\n" << infoLog);
        }
    
        GLCall(glValidateProgram(program));
    
        // Check for validation errors
        GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &success));
        if (!success) {
            char infoLog[1024];
            GLCall(glGetProgramInfoLog(program, 1024, nullptr, infoLog));
            LOG("[Shader Validation Error]:\n" << infoLog);
        }

        GLCall(glDeleteShader(_vertexShader));
        GLCall(glDeleteShader(_fragmentShader));

        return program;
    }

    uint32_t Shader::CompileShader(uint32_t type, const std::string& source)
    {
        uint32_t id = glCreateShader(type);
        const char* src = source.c_str();
        GLCall(glShaderSource(id, 1, &src, nullptr));
        GLCall(glCompileShader(id));

        GLint success;
        GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            char infoLog[1024];
            GLCall(glGetShaderInfoLog(id, 1024, NULL, infoLog));
            LOG("[Shader Compilation Error] (" << m_filePath << "):\n" << infoLog);
        }

        return id;
    }

    int Shader::GetUniformLocation(const std::string& uniform)
    {
        GLCall(int location = glGetUniformLocation(m_shader, uniform.c_str()))
        return location;
    }

    void Shader::Delete()
    {
        if (m_shader != -2) {
            GLCall(glDeleteProgram(m_shader));
            m_shader = -2;
        }
    }
}
