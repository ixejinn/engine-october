#pragma once
#include "../EditmodeUpdatable.h"

class Updatable : public EditmodeUpdatable
{
protected:
	virtual void Update() = 0;

public:
	virtual ~Updatable();

	friend class ComponentManager;
};