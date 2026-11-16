#pragma once

#include <glm.hpp>

struct Camera
{
	glm::vec3 front{0,0,-1}; //朝向
	glm::vec3 up{0,1,0}; //顶部
	float fov = 45.0f; //垂直视场角
	float Znear = 0.1f; //近裁剪面
	float Zfar = 100.0f; //远裁剪面

	bool IsMainCamera = false; //是否为主摄像机

	glm::mat4 Get_View(const glm::vec3& position) const;

	glm::mat4 Get_Projection(float aspect) const;
};
