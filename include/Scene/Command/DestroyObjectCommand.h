#pragma once

#include "ICommand/ICommand.h"
#include <cstdint>

namespace SceneManager::Command
{
	class DestroyObjectCommand : public SceneManager::ICommand
	{
	private:
		uint32_t ObjID;

	public:
		explicit DestroyObjectCommand(uint32_t ID);

		void execute(Scene& scene) override final;
	};
}
