#include "GameObject.h"

#include <iostream>
#include "../Manager/ComponentManager.h"
#include "../Component/Component.h"
#include "../Component/FixedUpdatable/Transform.h"

namespace Manager
{
    extern ComponentManager& compMgr;
}

GameObject::GameObject(const std::string& name) : name_{name}
{
    AddComponent(typeid(Transform));
}

GameObject::~GameObject() {}

Component* GameObject::AddComponent(std::type_index compType)
{
    components_.insert({ compType, std::unique_ptr<Component>(Manager::compMgr.CreateComponent(compType, this)) });
    return components_[compType].get();
}

Component* GameObject::AddComponent(std::string compName)
{
    return AddComponent(Manager::compMgr.nameToType_.find(compName)->second);
}

Component* GameObject::GetComponent(std::type_index compType)
{
    if (components_.find(compType) == components_.end())
    {
        std::cout << "GameObject::GetComponent() " << name_ << " does not have " << compType.name() << " component" << std::endl;
        return nullptr;
    }
    return components_[compType].get();
}

bool GameObject::HasComponent(std::type_index compType)
{
    if (components_.find(compType) == components_.end())
        return false;
    return true;
}

void GameObject::SetName(const std::string& newName)
{
    name_ = newName;
}

void GameObject::SetCollidable(Collidable* collidable)
{
    if (collidable_ != nullptr)
        std::cout << "GameObject::SetCollidable Change " << name_ << "'s collidable" << std::endl;

    collidable_ = collidable;
}

void GameObject::DeleteComponent(std::type_index compType)
{
    components_.erase(compType);
}
