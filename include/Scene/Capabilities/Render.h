#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "ICapabilities/ICapabilities.h"
#include "Core/SceneContants.h"
#include "Core/Scene.h"
#include "DataComponents/Mesh.h"
#include "Shader.h"
#include "ReadingFile.h"
#include "DataComponents/Camera.h"

namespace SceneManager::Capability
{
	class Render : public SceneManager::ICapabilities
	{
	private:
		static Mesh CreateQuadMesh(float width, float height);

		std::unique_ptr<Rendering::Shader> m_shader;

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