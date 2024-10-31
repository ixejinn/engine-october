#include "GameObjectManager.h"

#include <iostream>
#include "../GameObject/GameObject.h"

GameObject* GameObjectManager::CreateObject()
{
	return CreateObject("Object" + std::to_string(noNameObject_++));
}

GameObject* GameObjectManager::CreateObject(const std::string& name)
{
	std::unique_ptr<GameObject> newObj{ new GameObject(name) };
	objects_.insert({ name, std::move(newObj) });
	return objects_[name].get();
}

void GameObjectManager::RenameObject(const GameObject* const obj, const std::string& newName)
{
	for (auto it = objects_.begin(); it != objects_.end(); ++it)
	{
		if (it->second.get() == obj)
		{
			std::unique_ptr<GameObject> tmp = std::move(it->second);
			objects_.erase(it);

			tmp->SetName(newName);
			objects_.insert({ newName, std::move(tmp) });

			return;
		}
	}
}

void GameObjectManager::RenameObject(const std::string& name, const std::string& newName)
{
	auto it = objects_.find(name);
	if (it != objects_.end())
	{
		std::unique_ptr<GameObject> tmp = std::move(it->second);
		objects_.erase(it);

		tmp->SetName(newName);
		objects_.insert({ newName, std::move(tmp) });
	}
}

void GameObjectManager::DeleteObject(const GameObject* const obj)
{
	for (auto it = objects_.begin(); it != objects_.end(); ++it)
	{
		if (it->second.get() == obj)
		{
			objects_.erase(it);
			return;
		}
	}
}

void GameObjectManager::DeleteObject(const std::string& name)
{
	auto it = objects_.find(name);
	if (it != objects_.end())
		objects_.erase(it);
}

void GameObjectManager::Clear()
{
	objects_.clear();
	noNameObject_ = 0;
}
