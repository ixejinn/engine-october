#pragma once

class GameObject;

class Component
{
protected:
	GameObject* owner_ = nullptr;

	Component(GameObject* owner) : owner_(owner) {}

public:
	virtual ~Component() = default;

	GameObject* GetOwner() { return owner_; }
};