#pragma once
#include "ICommand/ICommand.h"
#include "ICapabilities/ICapabilities.h"
#include "GameObject.h"

#include <algorithm>
#include <any>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <typeindex>
#include <memory>
#include <queue>
#include <array>
#include <functional>
#include <type_traits>


//测试用,后续删除
namespace Test
{
	void Integration_Test_Run();
}


namespace SceneManager
{

	class Scene
	{
		//测试用,后续删除
		friend void ::Test::Integration_Test_Run();


	private:
		std::string name = "unnamed scene";
		std::vector<ICapabilities*> capabilities;
		std::unordered_map<uint32_t, GameObject> objects;
		std::unordered_map<uint32_t,std::vector<std::type_index>> componentTypes;
		std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::any>> componentPools;
		uint32_t GameObject_nextId = 0;
		bool active = true;
		std::queue<std::unique_ptr<ICommand>> CommandQueue;


		//卸载所有Capability
		void ClearAllCapabilities();

		void ClearAllGameObjects();//删除所有对象

		//清空组件池
		void ClearAllComponents();

	public:
		//初始化/销毁
		Scene() {};
		virtual ~Scene() = default;


		//GameObject管理
		uint32_t CreateObject(std::string name);
		void DestroyObject(uint32_t id);//删除单个对象及其所有组件
		SceneManager::GameObject* GetObject(uint32_t id);//获取单个对象
		unsigned long long GetObjectCount();
		const std::unordered_map<uint32_t, GameObject>& GetObjects();//遍历所有对象
		

		//Component管理
		//添加组件
		template<typename T>
		void AddComponent(uint32_t gameObjectId, T component)
		{
			//第一步:在ComponentPool中添加
			auto poolIt = componentPools.find(std::type_index(typeid(T)));
			bool insertsuccess = false;

			if (poolIt != componentPools.end())
			{
				auto& pool = poolIt->second;

				auto result = pool.emplace(gameObjectId, std::move(component));
				insertsuccess = result.second;
			}
			else
			{
				std::unordered_map<uint32_t, std::any> newpool;
				newpool.emplace(gameObjectId, std::move(component));

				componentPools.emplace(std::type_index(typeid(T)),std::move(newpool));
				insertsuccess = true;
			}

			//第二步:在ComponentType中添加

			if (insertsuccess == true)
			{
				componentTypes[gameObjectId].push_back(std::type_index(typeid(T)));
			}
			else
			{
				//日志:添加组件失败(已有该组件)
				return;
			}
		}
		//移除单个组件
		void RemoveComponent(uint32_t gameObjectId,std::type_index type);
		
		//查询单个GameObject中的组件
		const std::vector<std::type_index>& GetComponentTypes(uint32_t gameObjectId);
		//批量获取单个GameObject的所有组件
		template<typename... T>
		bool GetComponent(uint32_t gameObjectId, T*&... components)
		{
			auto find_in_pool = [this](uint32_t id, auto* type_tag) -> void*
				{
					using CurrentComponentType = std::remove_pointer_t<decltype(type_tag)>;
					//std::cout << "Looking for " << std::type_index(typeid(CurrentComponentType)).name() << "\n";

					auto poolIt = componentPools.find(std::type_index(typeid(CurrentComponentType)));

					if (poolIt != componentPools.end())
					{
						auto& pool = poolIt->second;
						auto componentIt = pool.find(id);

						if (componentIt != pool.end())
						{
							return std::any_cast<CurrentComponentType>(&componentIt->second);
						}
					}
					return nullptr;
				};

			((components = static_cast<T*> (find_in_pool(gameObjectId, static_cast<T*>(nullptr)))), ...);

			bool all_found = ((components != nullptr) &&...);

			if (!all_found)
			{
				((components = nullptr), ...);

				return false;
			}

			return true;
		}
		//批量获取单个组件类型的所有引用
		template<typename T>
		std::unordered_map<uint32_t, std::any>* GetPool()
		{
			auto poolIt = componentPools.find(std::type_index(typeid(T)));

			if (poolIt != componentPools.end())
			{
				return &poolIt->second;
			}
		
			return nullptr;
		}
		//交集查询
		template<typename... Components,typename Func>
		void ForEach(Func callback)
		{
			//动态获取最小池
			constexpr size_t count = sizeof...(Components);

			if constexpr (count == 0) return;


			if (((GetPool<Components>() == nullptr) || ...)) return;
			std::array<size_t, count> sizes = { GetPool<Components>()->size()... };

			auto min_sizeIt = min_element(sizes.begin(), sizes.end());

			if (*min_sizeIt == 0) return;

			int min_index = std::distance(sizes.begin(), min_sizeIt);//知道了最小池的索引

			std::array<const std::unordered_map<uint32_t, std::any>*, count> pools = { GetPool<Components>()... };//获取所有池的引用

			const auto* min_pool = pools[min_index];

			for (const auto& [gameobjectId, _] : *min_pool)
			{
				bool has_all_components = ((GetPool<Components>()->find(gameobjectId) != GetPool<Components>()->end()) && ...);

				if (has_all_components)
				{
					auto fetch_components = [this](uint32_t id, auto type_tag) -> auto&
						{
							using T = decltype(type_tag);

							auto& PoolIt = componentPools.at(std::type_index(typeid(T)));

							return std::any_cast<T&>(PoolIt.at(id));
						};

					callback(gameobjectId,fetch_components(gameobjectId,Components{})...);

				}
			}
		}
		

		//Capabilities管理  
		//挂载Capability
		void AddCapability(ICapabilities* ICapability);
		//卸载单个Capability
		void RemoveCapability(ICapabilities* ICapability);

		//遍历所有Capabilities
		const std::vector<ICapabilities*>& GetCapabilities();

		void Update(float datetime);


		//场景生命周期
		void SceneBegin();
		void SceneEnd();


		//查看预分配到的GamObjectID
		uint32_t PeekNextId() const
		{
			return GameObject_nextId;
		};

		template<typename T_command ,typename... Args>
		void PushCommand(Args&&... args)
		{
			CommandQueue.push(std::make_unique<T_command>(std::forward<Args>(args)...));
		}

		void FlushCommand();
	};
}
