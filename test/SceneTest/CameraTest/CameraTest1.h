#pragma once

#include "Capabilities/CameraController.h"
#include "MeshFactory.h"
#include "TestFramework.h"
#include "Core/Scene.h"
#include "DataComponents/Camera.h"
#include "Command/CreateObjectCommand.h"
#include "Capabilities/Render.h"
#include <cmath>
#include <glm.hpp>

namespace Test
{
	void CameraTest1_Run(SceneManager::Scene& scene);
}
