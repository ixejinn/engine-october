#include "GameObject.h"

#include <iostream>
#include "../Manager/ComponentManager.h"
#include "../Component/Component.h"
#include "../Component/FixedUpdatable/Transform.h"

namespace Manager
{
    extern ComponentManager& compMgr;
}

GameObject::GameObject(const std::string& name) : components_(), name_(name)
{
    AddComponent(typeid(Transform));
}

GameObject::~GameObject() {}

Component* GameObject::AddComponent(std::type_index type)
{
    components_.insert({ type, std::unique_ptr<Component>(Manager::compMgr.CreateComponent(type, this)) });
    return components_[type].get();
}

Component* GameObject::GetComponent(std::type_index compType)
{
    if (components_.find(compType) == components_.end())
    {
        std::cerr << "[ERROR] GameObject::GetComponent() " << name_ << " is missing a required component" << std::endl;
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

void GameObject::DeleteComponent(std::type_index compType)
{
    components_.erase(compType);
}
