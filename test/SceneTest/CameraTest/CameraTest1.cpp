//本测试文件测试内容为:测试GameObject能否正常挂载Camera组件,以及Camera组件的视图矩阵和投影矩阵是否正确计算

#include "SceneTest/CameraTest/CameraTest1.h"

namespace Test
{
	void Test::CameraTest1_Run(SceneManager::Scene& scene)
	{
		scene.PushCommand<SceneManager::Command::CreateObjectCommand>("CameraObject");
		scene.PushCommand<SceneManager::Command::CreateObjectCommand>("MeshObject");

		scene.FlushCommand();

		

		Camera camera;
		camera.IsMainCamera = true;

		scene.AddComponent<Camera>(0, camera);

		TEST_ASSERT(scene.GetComponentTypes(0).size() == 1, "GameObject 0 should have 1 component");
		//相机组件成功挂载

		Mesh mesh = Rendering::MeshFactory::CreateRectangle();

		scene.AddComponent<Mesh>(1, mesh);
		
		scene.GetObject(1)->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		scene.ForEach<Camera>([&scene](uint32_t id, Camera& camera)
			{
				if (camera.IsMainCamera)
				{
					glm::vec3 positionIt = scene.GetObject(id)->position;
					glm::mat4 viewMatrix = camera.Get_View(positionIt);
					glm::mat4 projectionMatrix = camera.Get_Projection(1.0f);
					//检查视图矩阵和投影矩阵是否正确计算
					TEST_ASSERT(viewMatrix[3][2] == -3.0f, "View matrix calculation is incorrect");
					TEST_ASSERT(projectionMatrix[1][1] == 1.0f / tan(glm::radians(camera.fov) / 2.0f), "Projection matrix calculation is incorrect");
				}
			});

		SceneManager::Capability::Render* renderCapability = new SceneManager::Capability::Render();
		scene.AddCapability(renderCapability);

		SceneManager::Capability::CameraController* cameraControllerCapability = new SceneManager::Capability::CameraController(glfwGetCurrentContext());
		scene.AddCapability(cameraControllerCapability);

	}
}

