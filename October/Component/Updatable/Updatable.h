#pragma once
#include "../Component.h"
#include "../EditmodeUpdatable.h"

class Updatable : public Component, public EditmodeUpdatable
{
protected:
	Updatable(GameObject* owner) : Component(owner) {}

	virtual void Update() = 0;

public:
	virtual ~Updatable();

	friend class ComponentManager;
};