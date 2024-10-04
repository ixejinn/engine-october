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

void GameObjectManager::RemoveObject(const GameObject* const obj)
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

void GameObjectManager::RemoveObject(const std::string& name)
{
	auto it = objects_.find(name);
	if (it != objects_.end())
		objects_.erase(it);
	else
		std::cout << "GameObjectManager::RemoveObject Invalid object name " + name << std::endl;
}

void GameObjectManager::Clear()
{
	for (auto it = objects_.begin(); it != objects_.end(); )
		objects_.erase(it++);

	noNameObject_ = 0;
}
