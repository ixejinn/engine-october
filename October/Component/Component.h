#pragma once
#include "../Serializer/ComponentSerializer.h"

class GameObject;

class Component : public ComponentSerializer
{
protected:
	GameObject* owner_ = nullptr;

	Component(GameObject* owner) : owner_(owner) {}

public:
	virtual ~Component() = default;

	GameObject* GetOwner() { return owner_; }

	virtual void ShowDetails() = 0;
};