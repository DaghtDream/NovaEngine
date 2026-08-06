#include "SceneTest/Test_Capabilities.h"

namespace Test
{
	void Test_Capabilities_Run()
	{
		SceneManager::Scene scene;

		SceneManager::Capability::MockCapability* mockcapability = new SceneManager::Capability::MockCapability();
		SceneManager::Capability::MockCapability2* mockcapability2 = new SceneManager::Capability::MockCapability2();
		SceneManager::Capability::MockCapability3* mockcapability3 = new SceneManager::Capability::MockCapability3();

		scene.AddCapability(mockcapability);
		TEST_ASSERT(scene.GetCapabilities().size() == 1, "First Capability add success");
		scene.AddCapability(mockcapability2);
		TEST_ASSERT(scene.GetCapabilities().size() == 2, "Second Capability add success");
		scene.AddCapability(mockcapability3);
		TEST_ASSERT(scene.GetCapabilities().size() == 3, "Third Capability add success");

		std::vector<SceneManager::ICapabilities*> capabilities = scene.GetCapabilities();

		for (const auto& capability : capabilities)
		{
			std::cout << "m_priority is:" << capability->GetPriority() << std::endl;
		}

		scene.SceneBegin();
		TEST_ASSERT(mockcapability->AttachCount == 1, "mockcapability Attach success");
		TEST_ASSERT(mockcapability2->AttachCount == 1, "mockcapability2 Attach success");
		TEST_ASSERT(mockcapability3->AttachCount == 1, "mockcapability3 Attach success");

		scene.Update(0.016f);
		TEST_ASSERT(mockcapability->UpdateCount == 1, "mockcapability Update success");
		TEST_ASSERT(mockcapability2->UpdateCount == 1, "mockcapability2 Update success");
		TEST_ASSERT(mockcapability3->UpdateCount == 1, "mockcapability3 Update success");

		scene.RemoveCapability(mockcapability2);
		TEST_ASSERT(scene.GetCapabilities().size() == 2, "remove capability success");

		scene.Update(0.016f);
		TEST_ASSERT(mockcapability->UpdateCount == 2, "mockcapability Update success");
		TEST_ASSERT(mockcapability3->UpdateCount == 2, "mockcapability3 Update success");

		scene.SceneEnd();
		TEST_ASSERT(mockcapability->DetachCount == 1, "mockcapability Detach success");
		TEST_ASSERT(mockcapability3->DetachCount == 1, "mockcapability3 Detach success");

		scene.RemoveCapability(mockcapability);
		scene.RemoveCapability(mockcapability3);
		TEST_ASSERT(scene.GetCapabilities().size() == 0, "Remove all capability success");

		PrintTestSummary();

		/*delete(mockcapability);
		delete(mockcapability2);
		delete(mockcapability3);*/
	}
}