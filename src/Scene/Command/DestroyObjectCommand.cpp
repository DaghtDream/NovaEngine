#include "Command/DestroyObjectCommand.h"
#include "Core/Scene.h"

namespace SceneManager::Command
{
	DestroyObjectCommand::DestroyObjectCommand(uint32_t ID) : ObjID(ID) {}

	void DestroyObjectCommand::execute(Scene& scene)
	{
		scene.DestroyObject(ObjID);
	}
}