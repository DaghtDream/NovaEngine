#pragma once

#include "ICommand/ICommand.h"
#include <string>

namespace SceneManager::Command
{
	//创建GameObject
	class CreateObjectCommand : public SceneManager::ICommand
	{
	private:
		std::string ObjName;

	public:
		//构造函数
		explicit CreateObjectCommand(std::string name);

		void execute(Scene& scene) override final;
	};
}