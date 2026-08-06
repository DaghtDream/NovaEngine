#pragma once
#include "TestFramework.h"
#include "Core/Scene.h"
#include "DataComponents/Lighting/PointLight.h"
#include "DataComponents/Mesh.h"
#include <cmath>
#include "glm.hpp"
#include "Command/CreateObjectCommand.h"
#include "MeshFactory.h"
#include "Capabilities/Render.h"
#include "Capabilities/ColorBlink.h"


namespace Test
{
	void stressTest_1_Run(SceneManager::Scene& scene);
}
