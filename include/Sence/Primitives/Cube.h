#pragma once

#include <memory>
#include <glm.hpp>
#include "Core/GameObject.h"



namespace SenceManger
{
	class GameObject;
}

namespace Primitives
{
	std::shared_ptr<SenceManger::GameObject> CreateCube(
		const glm::vec3& position = glm::vec3(0.0f),
		const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		const glm::vec3& scale = glm::vec3(1.0f)
	);
}



