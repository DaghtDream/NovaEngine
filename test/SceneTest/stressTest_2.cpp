#include "SceneTest/stressTest_2.h"

namespace Test
{
	void stressTest_2_Run(SceneManager::Scene& scene,int part)
	{
			if(part!=1&&part!=2)
			{
				std::cerr << "Invalid part number. Please use 1 or 2." << std::endl;
				return;
			}

			std::mt19937 mt(12345);

			int x = 1000;
			int y = 100;
			int totalObjects = x * y;

			std::uniform_real_distribution<float> typeDist(0.0f, 1.0f);//随机决定是否添加PointLight组件

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
				if(part == 1 && typeDist(mt) < 0.1f) // 10%概率添加PointLight组件
				{
					scene.AddComponent<PointLight>(i, pointlight);
				}
				else
				if(part == 2 && typeDist(mt) < 0.01f) // 1%概率添加PointLight组件
				{
					scene.AddComponent<PointLight>(i, pointlight);
				}
			}

			float cellheight = 2.0f / y;
			float cellwidth = 2.0f / x;

			float objectheight = cellheight * 0.1f;
			float objectwidth = cellwidth * 0.1f;

			
			float maxjitterX = (cellwidth - objectwidth) / 2.0f;
			float maxjitterY = (cellheight - objectheight) / 2.0f;

			std::uniform_real_distribution<float> jitterDistX(-maxjitterX, maxjitterX);
			std::uniform_real_distribution<float> jitterDistY(-maxjitterY, maxjitterY);

			for (int i = 0; i < totalObjects; ++i)
			{
				int row = i / x;
				int col = i % x;

				float xPos = -1.0f + col * cellwidth + cellwidth * 0.5f + jitterDistX(mt);
				float yPos = 1.0f - row * cellheight + cellheight*0.5f + jitterDistY(mt);

				glm::vec3 objectPosition = glm::vec3{ xPos, yPos, 0.0f };
				auto* obj = scene.GetObject(i);
				if (obj != nullptr)
				{
					obj->position = objectPosition;
					obj->scale = glm::vec3{ objectwidth, objectheight, 1.0f };
				}
			}

			SceneManager::Capability::Render* renderCapability = new SceneManager::Capability::Render();
			SceneManager::Capability::ColorBlink* colorBlinkCapability = new SceneManager::Capability::ColorBlink();
			scene.AddCapability(renderCapability);
			scene.AddCapability(colorBlinkCapability);
		
	}
}