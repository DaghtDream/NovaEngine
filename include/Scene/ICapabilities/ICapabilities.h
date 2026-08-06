#pragma once

namespace SceneManager
{
	class Scene;

	class ICapabilities
	{
	protected:
		Scene* m_scene = nullptr;
		int m_priority = 0; //默认优先级为0，数值越大优先级越低

	public:
		virtual ~ICapabilities() = default;
		virtual int GetPriority() const { return m_priority; } //默认优先级为0，数值越大优先级越低
		virtual void OnAttach(Scene* scene){}
		virtual void OnDetach(){}

		virtual void OnUpdate(float dt) = 0;

		virtual const char* GetName() const = 0;
	};
}