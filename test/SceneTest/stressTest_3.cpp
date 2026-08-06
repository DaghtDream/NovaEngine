#include "SceneTest/stressTest_3.h"

namespace Test
{
	void stressTest_3_Run(SceneManager::Scene& scene)
	{
		std::mt19937 PosMt(123456789);
		std::uniform_real_distribution<float> posDist(-1.0f, 1.0f);

		int x = 100;
		int y = 100;
		int totalObjects = x * y;
		for (int i = 0; i < totalObjects; ++i)
		{
			scene.PushCommand<SceneManager::Command::CreateObjectCommand>("unnamed entity");
		}

		scene.FlushCommand();

		Mesh mesh = Rendering::MeshFactory::CreateRectangle();
		PointLight pointlight;

		for (int i = 0; i < totalObjects; ++i)
		{
			scene.AddComponent<Mesh>(i, mesh);
			scene.AddComponent<PointLight>(i, pointlight);
		}



		for (int i = 0; i < totalObjects; ++i)
		{
			auto* obj = scene.GetObject(i);

			obj->position = glm::vec3{ posDist(PosMt), posDist(PosMt), 0.0f };
			obj->scale = glm::vec3{ 0.001f, 0.001f, 1.0f };
			
		}

		SceneManager::Capability::Render* renderCapability = new SceneManager::Capability::Render();
		SceneManager::Capability::ColorBlink* colorBlinkCapability = new SceneManager::Capability::ColorBlink();
		scene.AddCapability(renderCapability);
		scene.AddCapability(colorBlinkCapability);



	}
}