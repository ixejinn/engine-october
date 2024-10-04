#pragma once

class LateUpdatable
{
protected:
	virtual void LateUpdate() = 0;

	friend class ComponentManager;
};