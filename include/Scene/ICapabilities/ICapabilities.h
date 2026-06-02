#pragma once

namespace SceneManager
{
	class ICapabilities
	{
		class Scene;

	public:
		virtual ~ICapabilities() = default;

		virtual void OnAttach(Scene* scene){}
		virtual void OnDetach(){}

		virtual void OnUpdate(float dt) = 0;

		virtual const char* GetName() const = 0;
	};
}