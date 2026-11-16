#include "Shader.h"

namespace Rendering
{
	unsigned int Shader::CompileShader(unsigned int type, const char* source)
	{
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		int success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		return shader;
	}

	unsigned int Shader::CreateShaderProgram(const char* vertexSource, const char* fragmentSource)
	{
		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

		unsigned int program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		int success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return program;
	}
	
	Shader::Shader(Shader&& other) noexcept
	{
		m_ShaderProgram = other.m_ShaderProgram;
		m_uniform_Location = std::move(other.m_uniform_Location);
		other.m_ShaderProgram = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteProgram(m_ShaderProgram);
			m_ShaderProgram = other.m_ShaderProgram;
			m_uniform_Location = std::move(other.m_uniform_Location);
			other.m_ShaderProgram = 0;
		}
		return *this;
	}

	void Shader::Set_Uniform(const std::string& name,const glm::vec4& value)
	{
		int location;
		auto it = m_uniform_Location.find(name);
		if (it != m_uniform_Location.end())
		{
			location = it->second;
		}
		else
		{
			location = glGetUniformLocation(m_ShaderProgram, name.c_str());
			m_uniform_Location[name] = location;
		}
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::Set_Uniform(const std::string& name,const glm::mat4& value)
	{
		int location;
		auto it = m_uniform_Location.find(name);
		if (it != m_uniform_Location.end())
		{
			location = it->second;
		}
		else
		{
			location = glGetUniformLocation(m_ShaderProgram, name.c_str());
			m_uniform_Location[name] = location;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}