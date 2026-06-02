#pragma once

#include <string>
#include <cstdint>
#include <glm.hpp>
#include <gtc/quaternion.hpp>

namespace SceneManager
{
	class GameObject 
	{
	public:
		//GameObject的基础成员
		uint32_t id;//对象标识
		std::string name;//对象名称
		bool active{ true };//对象状态

		//对象transform
		glm::vec3 position{ 0 };
		glm::quat rotation{ 1,0,0,0 };
		glm::vec3 scale{ 1 };
	};
	
}


