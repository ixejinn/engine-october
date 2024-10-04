#pragma once
#include <map>
#include <typeindex>	// std::type_index
#include <memory>
#include <string>

class Component;

class GameObject
{
private:
	std::map<std::type_index, std::unique_ptr<Component>> components_;

	std::string name_;

	GameObject(const std::string& name);

public:
	~GameObject();

	Component* AddComponent(std::type_index type);
	Component* GetComponent(std::type_index type);

	void DeleteComponent(std::type_index type);

	friend class GameObjectManager;
};