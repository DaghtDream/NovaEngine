#include "Capabilities/CameraController.h"

namespace SceneManager::Capability
{
	void CameraController::OnAttach(Scene* scene)
	{
		m_scene = scene;

		//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwGetCursorPos(m_window, &m_lastX, &m_lastY);
	}

	void CameraController::OnUpdate(float dt)
	{
		if (!m_scene) return;

		m_scene->ForEach<Camera>([this,dt](uint32_t id, Camera& camera)
			{
				if (camera.IsMainCamera)
				{

					glm::vec3& cameraPosition = m_scene->GetObject(id)->position;

					if(glfwGetKey(m_window , GLFW_KEY_W) == GLFW_PRESS)
					{
						cameraPosition += glm::normalize(camera.front) * 1.1f * dt; // Move forward
					}
					
					if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
					{
						cameraPosition -= glm::normalize(camera.front) * 1.1f * dt; // Move backward	
					}

					if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
					{
						cameraPosition -= glm::normalize(glm::cross(camera.front, camera.up)) * 1.1f * dt; // Move left
					}

					if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
					{
						cameraPosition += glm::normalize(glm::cross(camera.front, camera.up)) * 1.1f * dt; // Move right
					}

					if(ImGui::GetIO().WantCaptureMouse)
					{
						return; // If ImGui wants to capture the mouse, don't update the camera
					}

					double xpos, ypos;
					glfwGetCursorPos(m_window, &xpos, &ypos);

					if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
					{


						float sensitivity = 0.05f;

						float xoffset = static_cast<float>(xpos - m_lastX) * sensitivity;
						float yoffset = static_cast<float>(m_lastY - ypos) * sensitivity;

						camera.front = glm::normalize(glm::angleAxis(glm::radians(-xoffset), camera.up) * camera.front);

						glm::vec3 right = glm::normalize(glm::cross(camera.front, camera.up));

						camera.front = glm::normalize(glm::angleAxis(glm::radians(yoffset), right) * camera.front);
					}

					m_lastX = xpos;
					m_lastY = ypos;

				}

			});
	}

	void CameraController::OnDetach()
	{
		m_scene = nullptr;
	}
}