#pragma once
#include "glm.hpp"

struct GPUPointLight
{
	glm::vec4 positionAndradius{ 1.0f, 1.0f, 1.0f, 1.0f };//{x,y,z,radius}
	glm::vec4 colorAndintensity{ 1.0f, 1.0f, 1.0f, 1.0f };//{r,g,b,intensity}

};
