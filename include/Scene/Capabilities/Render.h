#pragma once
#include "ICapabilities/ICapabilities.h" 
#include "Core/SceneContants.h"
#include "Core/Scene.h"
#include "Tool/ProfileBlock.h"

namespace SceneManager::Capability
{
	class Render : public SceneManager::ICapabilities
	{
	private:
		static std::string LoadShaderFile(const std::string& filePath);

		unsigned int m_shaderProgram{ 0 };
		int vertexModelLocation{ -1 };
		int vertexColorLocation{ -1 };

	public:

		Render()
		{
			m_priority = RENDER_PRIORITY_MIN;
		}

		~Render() override = default;

		int GetPriority() const override { return 101; } 

		void OnAttach(Scene* Scene) override;

		void OnUpdate(float dt) override;

		void OnDetach() override;

		const char* GetName() const override { return "Render"; }


	};
}
