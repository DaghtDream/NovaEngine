#pragma once
#include "ICapabilities/ICapabilities.h"
#include "Core/Scene.h"

namespace SceneManager::Capability
{
	class MockCapability : public SceneManager::ICapabilities
	{
	public:
		int AttachCount = 0;
		int DetachCount = 0;
		int UpdateCount = 0;

		MockCapability()
		{
			m_priority = 0;
		}

		~MockCapability() override = default;

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

		const char* GetName() const override { return "MockCapability"; }
	};
}