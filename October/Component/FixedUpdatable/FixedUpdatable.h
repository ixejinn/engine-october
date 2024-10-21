#pragma once
#include <chrono>
#include "../EditmodeUpdatable.h"

class FixedUpdatable : public EditmodeUpdatable
{
protected:
	virtual void FixedUpdate() = 0;

public:
	virtual ~FixedUpdatable();

	static constexpr const std::chrono::duration<long, std::milli> Step_{ 20 };

	friend class ComponentManager;
};