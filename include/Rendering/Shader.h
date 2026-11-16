#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>
#include <memory>

namespace Rendering
{
	class Shader
	{
	private:

		//编译着色器
		static unsigned int CompileShader(unsigned int type, const char* source);

		//链接着色器程序
		static unsigned int CreateShaderProgram(const char* vertexSource, const char* fragmentSource);

		std::unordered_map<std::string , int> m_uniform_Location;

		unsigned int m_ShaderProgram{ 0 };

	public:

		Shader(const char* vertex_source,const char* fragment_source)
		{
			m_ShaderProgram = CreateShaderProgram(vertex_source, fragment_source);
		};

		~Shader()
		{
			glDeleteProgram(m_ShaderProgram);
		};

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		//设置uniform变量
		void Set_Uniform(const std::string& name,const glm::vec4& value);
		void Set_Uniform(const std::string& name,const glm::mat4& value);

		void Use() const
		{
			glUseProgram(m_ShaderProgram);
		}	
	};
}