#pragma once
#include <map>
#include <typeindex>	// std::type_index
#include <memory>
#include <string>
#include <glm/vec2.hpp>

class Component;
class Collidable;

class GameObject
{
private:
	std::map<std::type_index, std::unique_ptr<Component>> components_{};
	std::string name_{};

	Component* mainComp_{ nullptr };
	Collidable* collidable_{ nullptr };

	GameObject(const std::string& name);

public:
	~GameObject();

	bool active_{ true };

	Component* AddComponent(std::type_index compType);
	Component* AddComponent(std::string compName);
	Component* GetComponent(std::type_index compType);

	const std::map<std::type_index, std::unique_ptr<Component>>& GetAllComponents() const { return components_; }
	int GetComponentNum() const { return int(components_.size()); }
	const std::string& GetName() const { return name_; }
	Component* GetMainComp() const { return mainComp_; }
	Collidable* GetCollidable() const { return collidable_; }

	bool HasComponent(std::type_index compType);

	void SetName(const std::string& newName);
	void SetMainComponent(Component* comp);
	void SetCollidable(Collidable* collidable);

	void InitializeComponent();
	void DeleteComponent(std::type_index type);

	friend class GameObjectManager;
};