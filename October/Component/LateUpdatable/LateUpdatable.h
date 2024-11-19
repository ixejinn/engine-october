#pragma once
#include "../Component.h"
#include "../EditmodeUpdatable.h"

class LateUpdatable : public Component, public EditmodeUpdatable
{
protected:
	LateUpdatable(GameObject* owner) : Component(owner) {}

	virtual void LateUpdate() = 0;

public:
	virtual ~LateUpdatable();

	friend class ComponentManager;
};