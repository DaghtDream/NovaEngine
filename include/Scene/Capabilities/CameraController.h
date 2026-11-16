#pragma once

#include <GLFW/glfw3.h>
#include "ICapabilities/ICapabilities.h"
#include "Core/Scene.h"
#include "DataComponents/Camera.h"
#include <glm.hpp>
#include <imgui.h>

namespace SceneManager::Capability
{
	class CameraController : public SceneManager::ICapabilities
	{
	private:
		GLFWwindow* m_window = nullptr;

		double m_lastX = 0.0;
		double m_lastY = 0.0;

	public:

		CameraController(GLFWwindow* window) : m_window(window)
		{
			m_priority = 100; //设置优先级为100，确保在渲染之前更新摄像机
		}

		~CameraController() override = default;

		int GetPriority() const override { return 100; }

		void OnAttach(Scene* scene) override;

		void OnUpdate(float dt) override;

		void OnDetach() override;

		const char* GetName() const override { return "CameraController"; }
	};
}