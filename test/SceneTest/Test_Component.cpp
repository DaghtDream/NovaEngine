#include "Test_Component.h"


namespace Test
{
	void Test_Component_Run()
	{
		SceneManager::Scene scene;

		scene.CreateObject("Test_1");
		scene.CreateObject("Test_2");
		scene.CreateObject("Test_3");

		//第一步基础组件操作
		Mesh mesh;
		scene.AddComponent(1, mesh);

		std::vector<std::type_index> types = scene.GetComponentTypes(1);

		TEST_ASSERT(types.size() == 1, "Should have 1 component");
		std::cout << std::endl;

		for (const auto& type : types)
		{
			std::cout << "--> " << type.name() << std::endl;
			TEST_ASSERT(type.name() == typeid(Mesh).name(), "should be Mesh type");
		}

		//对同一实体重复添加同一个组件
		scene.AddComponent(1, mesh);

		types = scene.GetComponentTypes(1);

		TEST_ASSERT(types.size() == 1, "should still have 1 component");

		for (const auto& type : types)
		{
			std::cout << " --> " << type.name() << std::endl;
			TEST_ASSERT(type.name() == typeid(Mesh).name(), "should be Mesh type");
		}

		//第二步多组件类型测试
		PointLight light;
		scene.AddComponent(1, light);
		types = scene.GetComponentTypes(1);

		TEST_ASSERT(types.size() == 2, "should have 2 components");
		std::cout << std::endl;

		for (const auto& type : types)
		{
			std::cout << " --> " << type.name() << std::endl;
			if (type.name() == typeid(Mesh).name())
			{
				TEST_ASSERT(true, "should be Mesh type");
			}
			else
			{
				TEST_ASSERT(type.name() == typeid(PointLight).name(), "should be PointLight type");
			}

		}

		scene.AddComponent(2, mesh);
		scene.RemoveComponent(1, std::type_index(typeid(light)));

		types = scene.GetComponentTypes(1);

		TEST_ASSERT(types.size() == 1, "Should have 1 component");
		std::cout << std::endl;

		for (const auto& type : types)
		{
			std::cout << " --> " << type.name() << std::endl;
			TEST_ASSERT(type.name() == typeid(Mesh).name(), "should be Mesh type");
		}

		std::unordered_map<uint32_t, std::any>* pools = scene.GetPool<Mesh>();

		if (pools)
		{
			for (const auto& [id, anyValue] : *pools)
			{
				std::cout << "Mesh owner ID: " << id << "\n";
			}
		}

		PrintTestSummary();
	}

}

