#include "Core/Scene.h"

namespace SceneManager
{
	Scene::Scene(){}
	Scene::~Scene(){}

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

	unsigned long long Scene::GetObjectCount()
	{
		return objects.size();
	}

	const std::unordered_map<uint32_t, GameObject>& Scene::GetObject()
	{
		return objects;
	}

	void Scene::ClearAllGameObjects()
	{
		componentPools.clear();
		componentTypes.clear();
		objects.clear();

		GameObject_nextId = 0;
	}


	//Component管理

}