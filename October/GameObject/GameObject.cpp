#include "GameObject.h"

#include <iostream>
#include "../Manager/ComponentManager.h"
#include "../Component/Component.h"

namespace Manager
{
    extern ComponentManager& compMgr;
}

GameObject::GameObject(const std::string& name) : components_(), name_(name) {}

GameObject::~GameObject() {}

Component* GameObject::AddComponent(std::type_index type)
{
    components_.insert({ type, std::unique_ptr<Component>(Manager::compMgr.CreateComponent(type, this)) });
    return components_[type].get();
}

Component* GameObject::GetComponent(std::type_index type)
{
    if (components_.find(type) == components_.end())
    {
        std::cerr << "[ERROR] GameObject::GetComponent() " << name_ << " is missing a required component" << std::endl;
        return nullptr;
    }
    return components_[type].get();
}

void GameObject::DeleteComponent(std::type_index type)
{
    components_.erase(type);
}
