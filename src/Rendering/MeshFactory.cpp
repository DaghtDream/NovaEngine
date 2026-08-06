#include <glad/glad.h>
#include "MeshFactory.h"

namespace Rendering
{
	Mesh MeshFactory::CreateRectangle()
	{
		Mesh mesh;

		float vertices[] = {
			0.5f,0.5f,0.0f,
			0.5f,-0.5f,0.0f,
			-0.5f,-0.5f,0.0f,
			-0.5f,0.5f,0.0f
		};

		unsigned int indices[] = {
			0,1,3,
			1,2,3
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
}