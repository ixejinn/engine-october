#pragma once
#include "../EditmodeUpdatable.h"

class FixedUpdatable : public EditmodeUpdatable
{
protected:
	virtual void FixedUpdate() = 0;

public:
	virtual ~FixedUpdatable();

	friend class ComponentManager;
};