#include "ComponentManager.h"

#include <iostream>
#include <chrono>
#include <typeindex>
#include "../GameObject/GameObject.h"
#include "../Component/Component.h"

#include "../Component/FixedUpdatable/FixedUpdatable.h"
#include "../Component/FixedUpdatable/Transform.h"

ComponentManager::ComponentManager()
{
	componentMap_.insert({ typeid(Transform), Transform::CreateComponent });
}

Component* ComponentManager::CreateComponent(std::type_index compType, GameObject* owner)
{
	if (componentMap_.find(compType) == componentMap_.end())
	{
		std::cerr << "Registry::CreateComponent() Invalid component type" << std::endl;
		return nullptr;
	}

	Component* newComp = componentMap_[compType](owner);

	FixedUpdatable* fixedComp = nullptr;
	if (fixedComp = dynamic_cast<FixedUpdatable*>(newComp))
		fixedComponents_.push_back(fixedComp);

	return newComp;
}

void ComponentManager::UpdateComponent()
{
	/* FIXED Update */
	static std::chrono::duration<long, std::milli> adder{ 0 };
	static std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	static std::chrono::system_clock::time_point cur = std::chrono::system_clock::now();

	adder = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start) + adder;
	while (adder >= FixedUpdatable::step)
	{
		for (auto it = fixedComponents_.begin(); it != fixedComponents_.end(); ++it)
		{
			(*it)->FixedUpdate();
		}

		adder -= FixedUpdatable::step;
	}

	/* Update */

	/* LATE Update */
}

void ComponentManager::DeleteComponent(std::type_index type)
{
}

void ComponentManager::Clear()
{
}
