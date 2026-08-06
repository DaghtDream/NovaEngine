#include "Core/Scene.h"

namespace SceneManager
{
	//GameObject管理
	uint32_t Scene::CreateObject(std::string name)
	{
		uint32_t newId = GameObject_nextId;
		GameObject_nextId++;

		objects.emplace(newId, GameObject{newId, std::move(name),true});

		return newId;
	}

	void Scene::DestroyObject(uint32_t id)
	{
		auto objIt = objects.find(id);
		if (objIt == objects.end())
		{
			return;
		}

		auto typelistIt = componentTypes.find(id);

		if (typelistIt != componentTypes.end())
		{
			const auto& types = typelistIt->second;

			for (const std::type_index& tIndex : types)
			{
				auto poolIt = componentPools.find(tIndex);
				if (poolIt != componentPools.end())
				{
					auto& pool = poolIt->second;
					pool.erase(id);
				}
			}

			componentTypes.erase(typelistIt);
		}

		objects.erase(objIt);
	}

	SceneManager::GameObject* Scene::GetObject(uint32_t id)
	{
		auto ObjectIt = objects.find(id);
		return ObjectIt != objects.end() ? &ObjectIt->second : nullptr;
	}

	unsigned long long Scene::GetObjectCount()
	{
		return objects.size();
	}

	const std::unordered_map<uint32_t, GameObject>& Scene::GetObjects()
	{
		return objects;
	}

	void Scene::ClearAllGameObjects()
	{
		objects.clear();
		GameObject_nextId = 0;
	}


	//Component管理
	void Scene::RemoveComponent(uint32_t gameObjectId, std::type_index type)
	{
		//第一步:删ComponentPool
		bool RemoveSuccess = false;
		auto poolIt = componentPools.find(type);

		if (poolIt != componentPools.end())
		{
			auto& pool = poolIt->second;
			if (pool.erase(gameObjectId))
			{
				RemoveSuccess = true;
			}
		}

		//第二步:删ComponentType
		if (RemoveSuccess)
		{
			auto typelistIt = componentTypes.find(gameObjectId);

			if (typelistIt != componentTypes.end())
			{
				auto& types = typelistIt->second;
				types.erase(std::remove(types.begin(), types.end(), type), types.end());

				if (types.empty())
				{
					componentTypes.erase(typelistIt);
				}
			}
		}
	}

	const std::vector<std::type_index>& Scene::GetComponentTypes(uint32_t gameObjectId)
	{
		auto typelistIt = componentTypes.find(gameObjectId);

		if (typelistIt == componentTypes.end())
		{
			static const std::vector<std::type_index> emptyTypes;
			return emptyTypes;
		}
		else
		{
			return typelistIt->second;
		}
	}

	void Scene::ClearAllComponents()
	{
		componentPools.clear();
		componentTypes.clear();
	}

	//Capability管理

	void Scene::AddCapability(ICapabilities* ICapability)
	{
		capabilities.push_back(ICapability);

		std::sort(capabilities.begin(), capabilities.end(), [](ICapabilities* a, ICapabilities* b)
			{
				return a->GetPriority() < b->GetPriority();
			});
	}

	void Scene::RemoveCapability(ICapabilities* ICapability)
	{
		ICapability->OnDetach();

		capabilities.erase(std::remove(capabilities.begin(), capabilities.end(), ICapability), capabilities.end());

		delete ICapability;
	}

	void Scene::ClearAllCapabilities()
	{
		for(auto it = capabilities.rbegin(); it != capabilities.rend(); ++it)
		{
			(*it)->OnDetach();
			//delete (*it);
		}

		capabilities.clear();
	}

	const std::vector<ICapabilities*>& Scene::GetCapabilities()
	{
		return capabilities;
	}

	//生命周期管理

	void Scene::Update(float deltaTime)
	{
		for(auto* capability : capabilities)
		{
			capability->OnUpdate(deltaTime);
		}

		FlushCommand();
	}

	void Scene::SceneBegin()
	{
		for(auto* capability : capabilities)
		{
			capability->OnAttach(this);
		}
	}

	void Scene::SceneEnd()
	{
		ClearAllCapabilities();

		FlushCommand();

		ClearAllComponents();
		ClearAllGameObjects();
	}

	void Scene::FlushCommand()
	{
		while (!CommandQueue.empty())
		{
			std::unique_ptr <ICommand> command = std::move(CommandQueue.front());
			CommandQueue.pop();

			command->execute(*this);

		}
	}

}