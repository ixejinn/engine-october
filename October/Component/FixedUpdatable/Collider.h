#pragma once
#include <glm/vec2.hpp>

#include "../Component.h"
#include "FixedUpdatable.h"

class Collider : public Component, public FixedUpdatable
{
protected:
	glm::vec2 center_ = { 0.f, 0.f };
};