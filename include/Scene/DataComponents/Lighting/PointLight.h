#pragma once
#include "glm.hpp"

struct PointLight
{
	glm::vec3 color{ 0.0f, 1.0f, 1.0f };

	float radius{ 1.0f };

	float intensity{ 1.0f };
};