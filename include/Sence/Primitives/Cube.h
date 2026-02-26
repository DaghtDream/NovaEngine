#pragma once

#include <memory>
#include <glm.hpp>
#include "Core/GameObject.h"
#include <string>

namespace SenceManger
{	
	class Sence;
}

namespace Primitives
{
	class Cube :public SenceManger::GameObject
	{
	public:
		//构造函数
		explicit Cube(
			const std::string& name = "Cube",
			bool active = true,
			SenceManger::Sence* sence = nullptr
		);

		//析构函数
		virtual ~Cube() = default;

		//更新函数
		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnDestroy() override;

	};
}



