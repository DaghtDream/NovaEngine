#pragma once

namespace SceneManager
{
	class Scene;

	class ICommand
	{
	public:
		virtual ~ICommand() = default;

		virtual void execute(Scene& Scene) = 0;

	};
}