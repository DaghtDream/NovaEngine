#include "SceneTest/Test_Component.h"
#include "SceneTest/Test_GameObject.h"
#include "SceneTest/Test_Capabilities.h"
#include "SceneTest/Integration_Test.h"
#include "SceneTest/stressTest_1.h"
#include "SceneTest/stressTest_2.h"
#include "SceneTest/stressTest_3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

bool InitialzeOpenGLTestContext()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW in test context!\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);


    GLFWwindow* window = glfwCreateWindow(1200, 1200, "NovaEngine Test", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create hidden GLFW window for tests!\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD in test context!\n";
        return false;
    }

    return true;
}

int main()
{
	//std::cout << "以下是gameobject部分的测试内容" << std::endl;

	//Test::Test_GameObject_Run();

	//std::cout << "以下是component部分的测试内容" << std::endl;

	//Test::Test_Component_Run();

	//std::cout << "以下是capability部分的测试内容" << std::endl;

	//Test::Test_Capabilities_Run();

	//std::cout << "以下是集成测试(Integration_Test)部分的测试内容" << std::endl;

    if (!InitialzeOpenGLTestContext()) {
        return -1; // 环境搭建失败，直接退出
    }

	//Test::Integration_Test_Run();

    SceneManager::Scene scene;

    //以下是压力测试1的测试内容
    //Test::stressTest_1_Run(scene);

	//以下是压力测试2的测试内容
    Test::stressTest_2_Run(scene, 1);

	//以下是压力测试3的测试内容
 //   Test::stressTest_3_Run(scene);

    scene.SceneBegin();

 //   int CreateCount = 500;
	//float spawnTimer = 0.0f;
	//float spawnInterval = 1.0f; // 每16ms创建一个对象

 //   uint32_t FindNextID = scene.PeekNextId();
 //   uint32_t DeadId = FindNextID - CreateCount;

 //   std::mt19937 PosMt(time(nullptr));
 //   std::uniform_real_distribution<float> posDist(-1.0f, 1.0f);

	//Mesh mesh = Rendering::MeshFactory::CreateRectangle();
	//PointLight pointlight;
	//bool Isfirstspawn = true;

	//std::vector<uint32_t> activeParticles;
	//activeParticles.reserve(6000);

 //   for (int i = 0; i < 5000; ++i)
 //   {
	//	scene.PushCommand<SceneManager::Command::CreateObjectCommand>("unnamed entity");
 //   }

 //   scene.FlushCommand();

 //   for (int i = 0; i < 5000; ++i)
 //   {
	//	uint32_t id = FindNextID + i;
 //       activeParticles.push_back(id);
 //       scene.AddComponent<Mesh>(id, mesh);
 //       scene.AddComponent<PointLight>(id, pointlight);
 //       auto* obj = scene.GetObject(id);
 //       if (obj != nullptr)
 //       {
 //           obj->position = glm::vec3{ posDist(PosMt), posDist(PosMt), 0.0f };
 //           obj->scale = glm::vec3{ 0.001f, 0.001f, 1.0f };
	//	}
 //   }

    while(!glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        //以下为压测3附加内容
        /*float dt = 0.016f;
		spawnTimer += dt;
        if (spawnTimer >= spawnInterval)
        {
            spawnTimer = 0.0f;

			uint32_t FindNextID = scene.PeekNextId();

            if (!Isfirstspawn)
            {
                {
					ProfileBlock timer("Destroy Objects");
                    for (int i = 0; i < CreateCount; ++i)
                    {
						std::uniform_int_distribution<int> dist(0, activeParticles.size() - 1);
						int deadIndex = dist(PosMt);

						scene.PushCommand<SceneManager::Command::DestroyObjectCommand>(activeParticles[deadIndex]);

						activeParticles[deadIndex] = activeParticles.back();
						activeParticles.pop_back();
                    }
                }
            }

            {
				ProfileBlock timer("Create Objects");
				FindNextID = scene.PeekNextId();
                for (int i = 0; i < CreateCount; ++i)
                {
                    scene.PushCommand<SceneManager::Command::CreateObjectCommand>("unnamed entity");
                }
            }

            {
				ProfileBlock timer("Flush Commands");
                scene.FlushCommand();
            }

            {
				ProfileBlock timer("Add Components");
                for (int i = 0; i < CreateCount; ++i)
                {
                    int AddId = FindNextID + i;
					activeParticles.push_back(AddId);
                    scene.AddComponent<Mesh>(AddId, mesh);
                    scene.AddComponent<PointLight>(AddId, pointlight);

                    auto* obj = scene.GetObject(AddId);
                    if (obj != nullptr)
                    {
                        obj->position = glm::vec3{ posDist(PosMt), posDist(PosMt), 0.0f };
                        obj->scale = glm::vec3{ 0.001f, 0.001f, 1.0f };
                    }
                }
            }
        }*/



        {
            ProfileBlock timer("Scene Update");
            scene.Update(0.016f); // 假设每帧更新约为16ms
        }

        
        {
            ProfileBlock timer("Scene Render");
            glfwSwapBuffers(glfwGetCurrentContext());
        }


        glfwPollEvents();
        
	}

	
	return 0;
}