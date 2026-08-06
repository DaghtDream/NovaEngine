#include "SceneTest/stressTest_1.h"

namespace Test
{
	void stressTest_1_Run(SceneManager::Scene& scene)
	{
		int y = 1000;
		int x = 100;
		int totalobjects = x * y;
		float spacing_x = 2.0f / x;
		float spacing_y = 2.0f / y;

		glm::vec3 objectScale = glm::vec3{ spacing_x * 0.8,spacing_y * 0.8,1 };

		for (int i = 0; i < totalobjects; ++i)
		{
			scene.PushCommand<SceneManager::Command::CreateObjectCommand>("unnamed entity"); 
		}

		scene.FlushCommand();

		for(int i = 0; i<totalobjects; ++i)
		{
			int row = i / x;
			int col = i % x;

			float xPos = -1.0f + (col * spacing_x) + (spacing_x / 2.0f);
			float yPos = 1.0f - (row * spacing_y) - (spacing_y / 2.0f);

			glm::vec3 objectPosition = glm::vec3{ xPos,yPos,0.0f };

			auto* obj = scene.GetObject(i);
			if (obj != nullptr)
			{
				obj->position = objectPosition;
				obj->scale = objectScale;
			}
		}

		Mesh mesh = Rendering::MeshFactory::CreateRectangle();
		PointLight pointlight;

		for (int i = 0; i < totalobjects; ++i)
		{
			scene.AddComponent<Mesh>(i, mesh);
			scene.AddComponent<PointLight>(i, pointlight);
			//std::cout << "Adding PointLight, type: " << std::type_index(typeid(PointLight)).name() << "\n";
		}
		TEST_ASSERT(scene.GetComponentTypes(1).size() == 2, "GameObject 1 should have 2 components");
		
		//std::unordered_map<uint32_t, std::any>* pointLightPool = scene.GetPool<PointLight>();
		//for (const auto& [id, anyvalue] : *pointLightPool)
		//{
		//	std::cout << id << std::endl;
		//}
		

		SceneManager::Capability::Render* renderCapability = new SceneManager::Capability::Render();
		scene.AddCapability(renderCapability);

		SceneManager::Capability::ColorBlink* colorBlinkCapability = new SceneManager::Capability::ColorBlink();
		scene.AddCapability(colorBlinkCapability);

		//TEST_ASSERT(scene.GetCapabilities().size() == 1, "Render capability should be added to the scene");

		
	}
}