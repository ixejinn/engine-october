#pragma once
#include "../Component.h"
#include "../EditmodeUpdatable.h"

class FixedUpdatable : public Component, public EditmodeUpdatable
{
protected:
	FixedUpdatable(GameObject* owner) : Component(owner) {}

	virtual void FixedUpdate() = 0;

public:
	virtual ~FixedUpdatable();

	friend class ComponentManager;
};