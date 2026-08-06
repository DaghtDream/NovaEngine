#pragma once
#include "TestFramework.h"
#include "Tool/ProfileBlock.h"
#include "Core/Scene.h"
#include "DataComponents/Lighting/PointLight.h"
#include "DataComponents/Mesh.h"
#include <cmath>
#include "glm.hpp"
#include "Command/CreateObjectCommand.h"
#include "Command/DestroyObjectCommand.h"
#include "MeshFactory.h"
#include "Capabilities/Render.h"
#include "Capabilities/ColorBlink.h"
#include <random>

namespace Test
{
	void stressTest_3_Run(SceneManager::Scene& scene);
}