#include "ComponentManager.h"

#include <iostream>
#include <chrono>
#include <typeinfo>
#include "../Editor/Editor.h"
#include "../Component/Component.h"

#include "../Profiler/Profiler.h"

#include "../Component/Transform.h"
#include "../Component/Rigidbody.h"
#include "../Component/FixedUpdatable/BoxCollider.h"
#include "../Component/FixedUpdatable/CircleCollider.h"
#include "../Component/LateUpdatable/Sprite.h"
#include "../Component/Updatable/PlayerController.h"
#include "../Component/Updatable/Gameplay/Player.h"

namespace Manager
{
	extern Editor& editor;
}

ComponentManager::ComponentManager()
{
	nameToType_.insert({ typeid(Transform).name(), typeid(Transform)});
	nameToType_.insert({ typeid(Rigidbody).name(), typeid(Rigidbody) });
	nameToType_.insert({ typeid(BoxCollider).name(), typeid(BoxCollider) });
	nameToType_.insert({ typeid(CircleCollider).name(), typeid(CircleCollider) });

	nameToType_.insert({ typeid(PlayerController).name(), typeid(PlayerController) });
	nameToType_.insert({ typeid(Player).name(), typeid(Player) });

	nameToType_.insert({ typeid(Sprite).name(), typeid(Sprite) });

	componentMap_.insert({ typeid(Transform), Transform::CreateComponent });
	componentMap_.insert({ typeid(Rigidbody), Rigidbody::CreateComponent });
	componentMap_.insert({ typeid(BoxCollider), BoxCollider::CreateComponent });
	componentMap_.insert({ typeid(CircleCollider), CircleCollider::CreateComponent });

	componentMap_.insert({ typeid(PlayerController), PlayerController::CreateComponent });
	componentMap_.insert({ typeid(Player), Player::CreateComponent });

	componentMap_.insert({ typeid(Sprite), Sprite::CreateComponent });
}

ComponentManager::~ComponentManager() {}

Component* ComponentManager::CreateComponent(std::type_index compType, GameObject* owner)
{
	if (componentMap_.find(compType) == componentMap_.end())
	{
		std::cerr << "ComponentManager::CreateComponent() Invalid component type" << std::endl;
		return nullptr;
	}

	if (owner->GetComponent(compType))
	{
		std::cout << "ComponentManager::CreateComponent() " << compType.name() << " already exists" << std::endl;
		return nullptr;
	}

	Component* newComp = componentMap_[compType](owner);

	if (compType == typeid(Transform))
		transforms_.push_back(static_cast<Transform*>(newComp));
	else if (compType == typeid(Rigidbody))
		rigidbodies_.push_back(static_cast<Rigidbody*>(newComp));
	else
	{
		FixedUpdatable* fixedComp = nullptr;
		Updatable* updComp = nullptr;
		LateUpdatable* lateComp = nullptr;

		if (fixedComp = dynamic_cast<FixedUpdatable*>(newComp))
			fixedComponents_.push_back(fixedComp);
		else if (updComp = dynamic_cast<Updatable*>(newComp))
			updComponents_.push_back(updComp);
		else if (lateComp = dynamic_cast<LateUpdatable*>(newComp))
			lateComponents_.push_back(lateComp);
	}

	return newComp;
}

void ComponentManager::FixedUpdate()
{
	for (auto& comp : fixedComponents_)
	{
		if (Manager::editor.GetMode() && !comp->updateInEditmode_)
			continue;

		comp->FixedUpdate();
	}
}

void ComponentManager::RigidbodyUpdate()
{
	for (auto& rb : rigidbodies_)
		rb->Update();
}

void ComponentManager::TransformUpdate()
{
	for (auto& trans : transforms_)
		trans->UpdateMatrix();
}

void ComponentManager::Update()
{
	DEBUG_PROFILER_BLOCK_START(__FUNCTION_NAME__);

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

	DEBUG_PROFILER_BLOCK_END;
}

void ComponentManager::LateUpdate()
{
	DEBUG_PROFILER_BLOCK_START(__FUNCTION_NAME__);

	for (auto it = lateComponents_.begin(); it != lateComponents_.end(); ++it)
	{
		if (Manager::editor.GetMode() && !(*it)->updateInEditmode_)
			continue;

		(*it)->LateUpdate();
	}

	DEBUG_PROFILER_BLOCK_END;
}

void ComponentManager::Clear()
{
	fixedComponents_.clear();
	rigidbodies_.clear();
	transforms_.clear();
	updComponents_.clear();
	lateComponents_.clear();
}
