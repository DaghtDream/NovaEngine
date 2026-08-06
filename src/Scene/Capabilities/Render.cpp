#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include "Capabilities/Render.h"
#include "DataComponents/Lighting/PointLight.h"
#include "DataComponents/Mesh.h"


namespace SceneManager::Capability
{
	std::string Render::LoadShaderFile(const std::string & filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open())
		{
			std::cerr << "Failed to open shader file: " << filePath << std::endl;
			return"";
		}

		std::string context(
			(std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		return context;
	}

	void Render::OnAttach(Scene* Scene)
	{
		m_scene = Scene;


		// Load and compile vertex shader
		std::string vertexSrc = LoadShaderFile("shader/default.vert");

		if (vertexSrc.empty())
		{
			std::cerr << "Vertex shader source is empty!" << std::endl;
			return;
		}

		const char* vertexPtr = vertexSrc.c_str();

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vertexPtr, nullptr);
		glCompileShader(vertexShader);

		int success;//×ÅÉ«Æ÷±àÒë×´Ì¬¼ì²é

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Load and compile fragment shader
		std::string fragSrc = LoadShaderFile("shader/default.frag");

		if (fragSrc.empty())
		{
			std::cerr << "Fragment shader source is empty!" << std::endl;
		}

		const char* fragPtr = fragSrc.c_str();

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &fragPtr, nullptr);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Link shaders into a program
		m_shaderProgram= glCreateProgram();
		glAttachShader(m_shaderProgram, vertexShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		glLinkProgram(m_shaderProgram);

		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		vertexModelLocation = glGetUniformLocation(m_shaderProgram, "uModel");
		vertexColorLocation = glGetUniformLocation(m_shaderProgram, "uColor");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Render::OnUpdate(float dt)
	{
		//std::cout << "Current Shader Program ID: " << m_shaderProgram << "\n";
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_shaderProgram);

		m_scene->ForEach<Mesh>([this](uint32_t id, Mesh& mesh) 
			{
			const auto& gameObject = m_scene->GetObjects();
			const GameObject& object = gameObject.at(id);

			PointLight* pointLight = nullptr;
			bool hasColor = m_scene->GetComponent(id, pointLight);

			if (!hasColor)
			{
				glUniform4f(vertexColorLocation, 0.8f, 0.8f, 0.8f, 1.0f);
				//std::cout << "don't have pointlight" << std::endl;
			}
			else
			{
				glUniform4f(vertexColorLocation, pointLight->color.r,pointLight->color.g,pointLight->color.b, 1.0f);
				//std::cout << "have pointlight" << std::endl;
			}

			glm::mat4 model = glm::translate(glm::mat4(1.0f), object.position)
							* glm::mat4_cast(object.rotation)
							* glm::scale(glm::mat4(1.0f), object.scale);
			
			glUniformMatrix4fv(vertexModelLocation, 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(mesh.VAO);

			glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);

			});
	}

	void Render::OnDetach()
	{
		glDeleteProgram(m_shaderProgram);
	}
}