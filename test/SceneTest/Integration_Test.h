#pragma once
#include "TestFramework.h"
#include "Core/Scene.h"
#include "Capabilities/Render.h"
#include "MeshFactory.h"
#include "DataComponents/Mesh.h"
#include "DataComponents/Lighting/PointLight.h"
#include "Command/CreateObjectCommand.h"
#include "Command/DestroyObjectCommand.h"

#include "SceneTest/MockCapability.h"
#include "SceneTest/MockCapability2.h"

namespace Test
{
	void Integration_Test_Run();
}
