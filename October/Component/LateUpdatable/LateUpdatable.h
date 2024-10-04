#pragma once

class LateUpdatable
{
protected:
	virtual void LateUpdate() = 0;

public:
	virtual ~LateUpdatable();

	friend class ComponentManager;
};