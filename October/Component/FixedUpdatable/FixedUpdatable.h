#pragma once
#include <chrono>

class FixedUpdatable
{
protected:
	virtual void FixedUpdate() = 0;

public:
	virtual ~FixedUpdatable();

	static constexpr const std::chrono::duration<long, std::milli> step{ 20 };

	friend class ComponentManager;
};