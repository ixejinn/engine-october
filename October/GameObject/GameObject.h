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

	Component* mainComp_ = nullptr;

	GameObject(const std::string& name);

public:
	~GameObject();

	Component* AddComponent(std::type_index compType);
	Component* GetComponent(std::type_index compType);

	const std::map<std::type_index, std::unique_ptr<Component>>& GetAllComponents() const { return components_; }
	const std::string& GetName() const { return name_; }
	Component* GetMainComp() const { return mainComp_; }

	bool HasComponent(std::type_index compType);

	void SetName(const std::string& newName);

	void DeleteComponent(std::type_index type);

	friend class GameObjectManager;
};