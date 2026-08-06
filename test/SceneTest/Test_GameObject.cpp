#include "Test_GameObject.h"

namespace Test
{
	void Test_GameObject_Run()
	{
		//第一步:场景初始化
		SceneManager::Scene scene;

		scene.CreateObject("Test_1");
		scene.CreateObject("Test_2");
		scene.CreateObject("Test_3");
		scene.CreateObject("Test_4");
		scene.CreateObject("Test_5");

		TEST_ASSERT(scene.GetObjectCount() == 5, "object count should be 5");

		//第二步:实体销毁测试
		scene.DestroyObject(1);
		scene.DestroyObject(3);

		scene.CreateObject("Test_6");
		scene.CreateObject("Test_7");

		scene.DestroyObject(999);

		//第三步:最终验证
		TEST_ASSERT(scene.GetObjectCount() == 5, "object count should be 5 after destroy and create");

		const auto& objects = scene.GetObjects();

		for (const auto& [id, obj] : objects)
		{
			std::cout << "Object ID: " << id << ", Name: " << obj.name << std::endl;
		}

		PrintTestSummary();
	}
}

