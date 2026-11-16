
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>

#include "Capabilities/Render.h"
#include "DataComponents/Lighting/PointLight.h"
#include "DataComponents/Mesh.h"

namespace SceneManager::Capability
{
	Mesh Render::CreateQuadMesh(float width, float height)
	{
		Mesh mesh;

		float hw = width * 0.5f;
		float hh = height * 0.5f;

		float vertices[] = {
			 hw,  hh, 0.0f,
			 hw, -hh, 0.0f,
			-hw, -hh, 0.0f,
			-hw,  hh, 0.0f
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		mesh.indexCount = sizeof(indices) / sizeof(unsigned int);

		glGenVertexArrays(1, &mesh.VAO);
		glBindVertexArray(mesh.VAO);

		glGenBuffers(1, &mesh.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &mesh.EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		return mesh;
	}

	void Render::OnAttach(Scene* Scene)
	{
		m_scene = Scene;

		std::string vertexSource = Rendering::ReadingFile::ReadFileToString("shader/default.vert");
		std::string fragmentSource = Rendering::ReadingFile::ReadFileToString("shader/default.frag");

		m_shader = std::make_unique<Rendering::Shader>(vertexSource.c_str(), fragmentSource.c_str());

	}

	void Render::OnUpdate(float dt)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shader->Use();

		m_scene->ForEach<Camera>([this](uint32_t id, Camera& camera)
			{
				if (camera.IsMainCamera == true)
				{   
					glm::vec3 positionIt = m_scene->GetObject(id)->position;
					glm::mat4 viewMatrix = camera.Get_View(positionIt);
					glm::mat4 projectionMatrix = camera.Get_Projection(1.0f);

					m_shader->Set_Uniform("uView", viewMatrix);
					m_shader->Set_Uniform("uProjection", projectionMatrix);
				}
			});

		m_scene->ForEach<Mesh>([this](uint32_t id, Mesh& mesh)
			{
				SceneManager::GameObject* object = m_scene->GetObject(id);
				if (object == nullptr)
				{
					return;
				}

				PointLight* pointLight = nullptr;
				bool hasLight = m_scene->GetComponent(id, pointLight);

				if (hasLight && pointLight != nullptr)
				{
					m_shader->Set_Uniform("uColor", glm::vec4(pointLight->color, 1.0f) * pointLight->intensity);
				}
				else
				{
					m_shader->Set_Uniform("uColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
				}

				glm::mat4 model = glm::translate(glm::mat4(1.0f), object->position)
					* glm::mat4_cast(object->rotation)
					* glm::scale(glm::mat4(1.0f), object->scale);

				m_shader->Set_Uniform("uModel", model);

				glBindVertexArray(mesh.VAO);
				glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
			});


	}

	void Render::OnDetach()
	{
		
	}
}