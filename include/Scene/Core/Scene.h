#pragma once
#include "ICommand/ICommand.h"
#include "ICapabilities/ICapabilities.h"
#include "GameObject.h"

#include <any>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <typeindex>
#include <memory>
#include <queue>

namespace SceneManager
{
	class Scene
	{
	private:
		std::string name = "unnamed scene";
		std::vector<ICapabilities*> capabilities;
		std::unordered_map<uint32_t, GameObject> objects;
		std::unordered_map<uint32_t,std::vector<std::type_index>> componentTypes;
		std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::any>> componentPools;
		uint32_t GameObject_nextId = 0;
		bool active = true;
		std::queue<std::unique_ptr<ICommand>> CommandQueue;

	public:
		//初始化/销毁
		Scene();
		virtual ~Scene() = default;


		//GameObject管理
		uint32_t CreateObject(std::string name);
		void DestroyObject(uint32_t id);//删除单个对象及其所有组件
		unsigned long long GetObjectCount();
		const std::unordered_map<uint32_t, GameObject>& GetObject();//遍历所有对象
		void ClearAllGameObjects();//删除所有对象及其组件


		//Component管理
		//添加组件
		template<typename T>
		void AddComponent(uint32_t gameObjectId, T component)
		{
			//第一步:在ComponentType中添加
			if (gameObjectId != componentTypes.find(gameObjectId))
			{
				componentTypes[gameObjectId].push_back(std::type_index(typeid(T)));
			}
			else
			{
				componentTypes[gameObjectId] = { std::type_index(typeid(T)) };
			}

			//第二步:在ComponentPool中添加
			auto poolIt = componentPools.find(std::type_index(typeid(T)));

			if (poolIt != componentPools.end())
			{
				auto& pool = poolIt->second;

				pool.emplace(gameObjectId, std::move(component));
			}
			else
			{
				std::unordered_map<uint32_t, std::any> newpool;
				newpool.emplace(gameObjectId, std::move(component));

				componentPools.emplace(std::type_index(typeid(T)),std::move(newpool));
			}
		}
		//移除组件
		void RemoveComponent(uint32_t gameObjectId,std::type_index type);
		//查询单个GameObject中的组件
		const std::vector<std::type_index>& GetComponentTypes(uint32_t gameObjectId);
		//批量获取单个GameObject的所有组件
		template<typename... T>
		bool GetComponent(uint32_t gameObjectId, T*&... components);
		//批量获取单个组件类型的所有引用
		template<typename T>
		const std::unordered_map<uint32_t, std::any>& GetPool();


		//Capabilities管理  
		//挂载Capability
		void AddCapability(ICapabilities* ICapability);
		//卸载单个Capability
		void RemoveCapability(ICapabilities* ICapability);
		//卸载所有Capability
		void ClearAllCapabilities();
		//遍历所有Capabilities
		const std::vector<ICapabilities*>& GetCapabilities();

		void Update(float datetime);


		//场景生命周期
		void SceneBegin();
		void SceneEnd();


		//查看预分配到的GamObjectID
		uint32_t PeekNextId() const;

		template<typename T_command ,typename... Args>
		void PushCommand(Args&&... args)
		{
			CommandQueue.push(std::make_unique<T_command>(std::forward<Args>(args)...));
		}

		void FlushCommand();
	};
}
