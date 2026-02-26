#include "Core/GameObject.h"
#include "Core/Transform.h"
#include "Core/Sence.h"
#include <algorithm>


namespace SenceManger
{
	//构造函数
	GameObject::GameObject(const std::string& name,bool active,Sence* sence):
		name(name),active(active),sence(sence){}

	//获取名称
	const std::string& GameObject::GetName() const
	{
		return name;
	}

	//设置名称
	void GameObject::SetName(const std::string& name)
	{
		this->name = name;
	}

	//获取是否激活
	const bool GameObject::IsActive() const
	{
		return active;
	}

	//设置是否激活
	void GameObject::SetActive(bool active)
	{
		this->active = active;
	}

	//获取变换组件
	Transform& GameObject::GetTransform()
	{
		return transform;
	}

	const Transform& GameObject::GetTransform() const
	{
		return transform;
	}

	//添加子对象
	void GameObject::AddChild(std::shared_ptr<GameObject> child)
	{
		if (!child || child.get() == this) return;
		// 如果 child 已有父对象，先从原父对象移除
		if (auto currentParent = child->GetParent()) {
			currentParent->RemoveChild(child);
		}
		// 直接设置子对象的父指针（弱引用）
		child->parent = weak_from_this();
		children.push_back(child);
	}

	//移除子对象
	void GameObject::RemoveChild(std::shared_ptr<GameObject> child)
	{
		if (!child) return;
		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end()) {
			children.erase(it);

			child->parent.reset();
		}
	}

	//获取子对象列表
	const std::vector<std::shared_ptr<GameObject>>& GameObject::GetChildren() const
	{
		return children;
	}

	//获取父对象
	const std::shared_ptr<GameObject> GameObject::GetParent() const
	{
		return parent.lock();
	}

	//设置父对象
	void GameObject::SetParent(std::shared_ptr<GameObject> newParent)
	{
		if (newParent == GetParent()) return;
		// 从原父对象移除自己
		if (auto oldParent = GetParent()) {
			oldParent->RemoveChild(shared_from_this());
		}
		// 添加到新父对象（如果非空）
		if (newParent) {
			newParent->AddChild(shared_from_this());
		}
		else {
			parent.reset();  // 注意：这里是成员变量 parent
		}
	}

	//更新函数
	void GameObject::OnStart()
	{
		//对象创建时调用一次
	}

	void GameObject::OnUpdate(float deltaTime)
	{
		//每帧调用一次，deltaTime为两帧之间的时间间隔
	}

	void GameObject::OnDestroy()
	{
		//对象销毁时调用一次
	}

	//设置所属场景
	void GameObject::SetSence(Sence* sence)
	{
		this->sence = sence;
	}

	//获取所属场景
	const Sence* GameObject::GetSence() const
	{
		return sence;
	}
}