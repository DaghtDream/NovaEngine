#pragma once

namespace SenceManger
{
	class GameObject;

	class Component
	{
	protected:
		friend class GameObject;
		GameObject* gameObject = nullptr; //所属游戏对象
	

	public:
		//析构函数
		virtual ~Component() = default;

		//获取所属游戏对象
		GameObject* GetGameObject() const
		{
			return gameObject;
		}

		//更新函数
		virtual void OnStart() {};
		virtual void OnUpdate(float deltaTime) {};
		virtual void OnDestroy() {};
	};
}