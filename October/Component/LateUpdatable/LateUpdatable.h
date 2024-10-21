#pragma once
#include "../EditmodeUpdatable.h"

class LateUpdatable : public EditmodeUpdatable
{
protected:
	virtual void LateUpdate() = 0;

public:
	virtual ~LateUpdatable();

	friend class ComponentManager;
};