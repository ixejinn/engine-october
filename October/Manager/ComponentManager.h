#pragma once
#include <map>
#include <typeindex>	// std::type_index
#include <list>
#include <type_traits>	// std::is_same

#include "../Component/FixedUpdatable/FixedUpdatable.h"
#include "../Component/LateUpdatable/LateUpdatable.h"

#include "../GameObject/GameObject.h"

class Component;

class ComponentManager
{
private:
	std::list<FixedUpdatable*> fixedComponents_;
	std::list<LateUpdatable*> lateComponents_;

	std::map<std::string, std::type_index> nameToType_;
	std::map<std::type_index, Component* (*)(GameObject* owner)> componentMap_;

	bool skipUpdate = false;

	ComponentManager();
	~ComponentManager();

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

	void SetSkipUpdate() { skipUpdate = true; }

	Component* CreateComponent(std::type_index compType, GameObject* owner);
	Component* CreateComponent(std::string compName, GameObject* owner);

	void UpdateComponent();

	// @tparam T: Type of component (FixedUpdatable, Updatable, LateUpdatable)
	template <typename T>
	void DeleteComponent(T* comp);

	void Clear();
};

template<typename T>
inline void ComponentManager::DeleteComponent(T* comp)
{
	if constexpr (std::is_same<T, FixedUpdatable>::value)
	{
		for (auto it = fixedComponents_.begin(); it != fixedComponents_.end(); ++it)
		{
			if (*it == comp)
			{
				fixedComponents_.erase(it);
				return;
			}
		}
	}
	else if constexpr (std::is_same<T, LateUpdatable>::value)
	{
		for (auto it = lateComponents_.begin(); it != lateComponents_.end(); ++it)
		{
			if (*it == comp)
			{
				lateComponents_.erase(it);
				return;
			}
		}
	}
	// Updatable, LateUpdatable Ãß°¡
}