#include <gtc/matrix_transform.hpp>

#include "DataComponents/Camera.h"

glm::mat4 Camera::Get_View(const glm::vec3& position) const
{
	glm::mat4 m_view = glm::lookAt(position, position + this->front, this->up);

	return m_view;
}

glm::mat4 Camera::Get_Projection(float aspect) const
{
	float radians = glm::radians(fov);

	glm::mat4 m_projection = glm::perspective(radians, aspect, this->Znear, this->Zfar);

	return m_projection;
}