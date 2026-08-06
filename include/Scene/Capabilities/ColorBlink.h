#pragma once
#include "ICapabilities/ICapabilities.h"
#include "Core/Scene.h"
#include "Core/SceneContants.h"
#include "DataComponents/Lighting/PointLight.h"
#include "Tool/ProfileBlock.h"

namespace SceneManager::Capability
{
	class ColorBlink : public ICapabilities
	{
	private:
		float m_time = 0.0f;

	public:
		ColorBlink() = default;
		virtual ~ColorBlink() = default;

		int GetPriority() const override { return RENDER_PRIORITY_MIN; }

		virtual void OnAttach(Scene* scene) override;

		virtual void OnUpdate(float dt) override;

		virtual void OnDetach() override;

		virtual const char* GetName() const override
		{
			return "ColorBlink";
		}
	};
}
