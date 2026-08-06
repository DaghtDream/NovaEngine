#include "SceneTest/Integration_Test.h"

namespace Test
{
	void Integration_Test_Run()
	{
		SceneManager::Scene scene;

		SceneManager::Capability::MockCapability* mockcapability = new SceneManager::Capability::MockCapability();
		SceneManager::Capability::MockCapability2* mockcapability2 = new SceneManager::Capability::MockCapability2();
		SceneManager::Capability::Render* render = new SceneManager::Capability::Render();

		scene.AddCapability(mockcapability);
		scene.AddCapability(mockcapability2);
		scene.AddCapability(render);

		scene.SceneBegin();

		scene.CreateObject("ID_0");
		scene.CreateObject("ID_1");
		scene.CreateObject("ID_2");
		scene.CreateObject("ID_3");
		scene.CreateObject("ID_4");
		scene.CreateObject("ID_5");
		scene.CreateObject("ID_6");
		scene.CreateObject("ID_7");
		scene.CreateObject("ID_8"); 
		scene.CreateObject("ID_9");

		Mesh mesh;
		scene.AddComponent(1, mesh);
		scene.AddComponent(3, mesh);
		scene.AddComponent(5, mesh);
		scene.AddComponent(7, mesh);
		scene.AddComponent(9, mesh);

		PointLight pointlight;
		scene.AddComponent(0, pointlight);
		scene.AddComponent(2, pointlight);
		scene.AddComponent(4, pointlight);

		scene.AddComponent(6, mesh);
		scene.AddComponent(6, pointlight);
		scene.AddComponent(8, mesh);
		scene.AddComponent(8, pointlight);

		TEST_ASSERT(scene.GetObjectCount() == 10, "objectcount should be 10");
		TEST_ASSERT(scene.GetPool<Mesh>()->size() == 7, "mesh pool count should be 7");
		TEST_ASSERT(scene.GetPool<PointLight>()->size() == 5, "pointlight pool count should be 7");

		int count = 0;
		scene.ForEach<Mesh, PointLight>([&count](uint32_t id, Mesh& mesh, PointLight& pointlight)
			{
				count++;
			});
		TEST_ASSERT(count == 2, "have mesh and pointlight components has 2 objects");

		scene.PushCommand<SceneManager::Command::CreateObjectCommand>("ID_10");
		scene.FlushCommand();

		TEST_ASSERT(scene.GetObjectCount() == 11, "CreateObjectCommand success");

		scene.PushCommand<SceneManager::Command::DestroyObjectCommand>(5);
		scene.Update(0.016f);
		TEST_ASSERT(scene.GetObjectCount() == 10, "DestroyObjectCommand success");

		scene.AddComponent(0, mesh);
		scene.RemoveComponent(6, std::type_index(typeid(pointlight)));

		TEST_ASSERT(scene.GetPool<Mesh>()->size() == 7, "7 objects have Mesh component now");
		TEST_ASSERT(scene.GetPool<PointLight>()->size() == 4, "4 objects have PointLight component now");

		for (int i = 0; i < 60; ++i)
		{
			scene.Update(0.016f);
			TEST_ASSERT(scene.GetObjectCount() == 10, "each update objectcount is 10");
		}
		
		TEST_ASSERT(mockcapability->UpdateCount == 61, "updatecount should be 61");

		scene.DestroyObject(1);
		scene.DestroyObject(3);
		scene.DestroyObject(7);

		TEST_ASSERT(scene.GetObjectCount() == 7, "now objects count should be 7");
		TEST_ASSERT(scene.GetPool<Mesh>()->size() == 4, "now should have 4 objects have Mesh component");

		scene.Update(0.016f);

		scene.SceneEnd();

		TEST_ASSERT(scene.GetObjectCount() == 0, "objectcount should be 0");
		TEST_ASSERT(scene.GetCapabilities().empty(), "Capability pool should be empty");

		if (scene.componentPools.empty())
		{
			TEST_ASSERT(true, "component pool is empty");
		}

		PrintTestSummary();

	}
}