#pragma once
#include "ICapabilities/ICapabilities.h"
#include "Core/Scene.h"

namespace SceneManager::Capability
{
	class MockCapability3 : public SceneManager::ICapabilities
	{
	public:
		int AttachCount = 0;
		int DetachCount = 0;
		int UpdateCount = 0;

		MockCapability3()
		{
			m_priority = 99;
		}

		~MockCapability3() override = default;

		int GetPriority() const override { return m_priority; }
		void OnAttach(Scene* Scene) override
		{
			m_scene = Scene;
			AttachCount++;
		}

		void OnUpdate(float dt) override
		{
			UpdateCount++;
		}

		void OnDetach() override
		{
			DetachCount++;
		}

		const char* GetName() const override { return "MockCapability3"; }
	};
}