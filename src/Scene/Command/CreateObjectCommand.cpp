#include "Command/CreateObjectCommand.h"
#include "Core/Scene.h"

namespace SceneManager::Command
{
	CreateObjectCommand::CreateObjectCommand(std::string name) : ObjName(std::move(name)) {}

	void CreateObjectCommand::execute(Scene& scene)
	{
		uint32_t ObjID = 0;
		ObjID = scene.CreateObject(ObjName);
	}
}