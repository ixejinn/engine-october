#pragma once
#include <map>
#include <typeindex>	// std::type_index
#include <list>
#include <functional>

class GameObject;
class Component;
class FixedUpdatable;

class ComponentManager
{
private:
	std::list<FixedUpdatable*> fixedComponents_;

	std::map<std::type_index, std::function<Component*(GameObject*)>> componentMap_;
	//std::map<std::type_index, Component* (*)(GameObject* owner)> componentMap_;

	void RegisterComponents();

	ComponentManager();
	~ComponentManager() = default;

	ComponentManager(const ComponentManager&) = delete;
	ComponentManager& operator =(const ComponentManager&) = delete;
	ComponentManager(ComponentManager&&) = delete;
	ComponentManager& operator =(ComponentManager&&) = delete;

public:
	static ComponentManager& GetInstance()
	{
		static ComponentManager instance;
		return instance;
	}
	
	Component* CreateComponent(std::type_index compType, GameObject* owner);

	void UpdateComponent();

	void DeleteComponent(std::type_index type);
	void Clear();
};