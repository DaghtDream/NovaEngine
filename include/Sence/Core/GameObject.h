#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm.hpp>
#include "Core/Transform.h"


namespace SenceManger
{
	class Sence;

	class GameObject : public std::enable_shared_from_this<GameObject>
	{
	private:
		std::string name;//名称
		bool active;//是否参与渲染和更新


		Transform transform;//变换组件


		std::vector<std::shared_ptr<GameObject>> children;//子对象列表
		std::weak_ptr<GameObject> parent;//父对象指针


		Sence* sence;//所属场景指针

	public:
		//构造函数
		explicit GameObject(
			const std::string& name = "GameObject",
			bool active = true,
			Sence* sence = nullptr
		);
		//析构函数
		virtual ~GameObject() = default;


		//获取名称
		const std::string& GetName() const;
		//设置名称
		void SetName(const std::string& name);


		//获取是否激活
		const bool IsActive() const;
		//设置是否激活
		void SetActive(bool active);


		//获取变换组件
		Transform& GetTransform();
		const Transform& GetTransform() const;


		//添加子对象
		void AddChild(std::shared_ptr<GameObject> child);
		//移除子对象
		void RemoveChild(std::shared_ptr<GameObject> child);
		//获取子对象列表
		const std::vector < std::shared_ptr<GameObject>>& GetChildren() const;
		//获取父对象
		const std::shared_ptr<GameObject> GetParent() const;
		//设置父对象
		void SetParent(std::shared_ptr<GameObject> parent);


		//更新函数
		virtual void OnStart();//对象创建时调用一次
		virtual void OnUpdate(float deltaTime);//每帧调用一次，deltaTime为两帧之间的时间间隔
		virtual void OnDestroy();//对象销毁时调用一次

		//设置所属场景
		void SetSence(Sence* sence);
		//获取所属场景
		const Sence* GetSence() const;
	};
}


