#include "ComponentManager.h"

#include <iostream>
#include <chrono>
#include <typeinfo>
#include "../Editor/Editor.h"
#include "../Component/Component.h"

#include "../Component/FixedUpdatable/Transform.h"
#include "../Component/FixedUpdatable/Rigidbody.h"
#include "../Component/LateUpdatable/Sprite.h"
#include "../Component/Updatable/PlayerController.h"

namespace Manager
{
	extern Editor& editor;
}

ComponentManager::ComponentManager()
{
	nameToType_.insert({ typeid(Transform).name(), typeid(Transform)});
	nameToType_.insert({ typeid(Rigidbody).name(), typeid(Rigidbody)});
	nameToType_.insert({ typeid(Sprite).name(), typeid(Sprite) });
	nameToType_.insert({ typeid(PlayerController).name(), typeid(PlayerController) });

	componentMap_.insert({ typeid(Transform), Transform::CreateComponent });
	componentMap_.insert({ typeid(Rigidbody), Rigidbody::CreateComponent });
	componentMap_.insert({ typeid(Sprite), Sprite::CreateComponent });
	componentMap_.insert({ typeid(PlayerController), PlayerController::CreateComponent });
}

ComponentManager::~ComponentManager() {}

Component* ComponentManager::CreateComponent(std::type_index compType, GameObject* owner)
{
	if (componentMap_.find(compType) == componentMap_.end())
	{
		std::cerr << "Registry::CreateComponent() Invalid component type" << std::endl;
		return nullptr;
	}

	Component* newComp = componentMap_[compType](owner);

	FixedUpdatable* fixedComp = nullptr;
	Updatable* updComp = nullptr;
	LateUpdatable* lateComp = nullptr;

	if (fixedComp = dynamic_cast<FixedUpdatable*>(newComp))
		fixedComponents_.push_back(fixedComp);
	else if (updComp = dynamic_cast<Updatable*>(newComp))
		updComponents_.push_back(updComp);
	else if (lateComp = dynamic_cast<LateUpdatable*>(newComp))
		lateComponents_.push_back(lateComp);

	return newComp;
}

Component* ComponentManager::CreateComponent(std::string compName, GameObject* owner)
{
	return CreateComponent(nameToType_.find(compName)->second, owner);
}

void ComponentManager::UpdateComponent()
{
	/* FIXED Update */
	static std::chrono::duration<long, std::milli> adder{ 0 };
	static std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

	std::chrono::system_clock::time_point cur = std::chrono::system_clock::now();
	adder += std::chrono::duration_cast<std::chrono::milliseconds>(cur - start);
	if (adder >= FixedUpdatable::Step_)
		start = cur;

	while (adder >= FixedUpdatable::Step_)
	{
		for (auto it = fixedComponents_.begin(); it != fixedComponents_.end(); ++it)
		{
			if (Manager::editor.GetMode() && !(*it)->updateInEditmode_)
				continue;

			(*it)->FixedUpdate();
		}

		adder -= FixedUpdatable::Step_;
	}

	/* Update */
	if (!Manager::editor.GetMode())
	{
		for (auto it = updComponents_.begin(); it != updComponents_.end(); ++it)
		{
			(*it)->Update();

			if (skipUpdate_)
				break;
		}
		skipUpdate_ = false;
	}

	/* LATE Update */
	for (auto it = lateComponents_.begin(); it != lateComponents_.end(); ++it)
	{
		if (Manager::editor.GetMode() && !(*it)->updateInEditmode_)
			continue;

		(*it)->LateUpdate();
	}
}

void ComponentManager::Clear()
{
	fixedComponents_.clear();
	updComponents_.clear();
	lateComponents_.clear();
}
