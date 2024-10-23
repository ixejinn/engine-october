#pragma once
#include <glm/vec2.hpp>

#include "../Component.h"
#include "FixedUpdatable.h"

class Transform;

class Collider : public Component, public FixedUpdatable
{
protected:
	glm::vec2 center_{ 0.f, 0.f };

	glm::vec2 vertices_[4]{};
	glm::vec2 bottomLeft_{};
	glm::vec2 topRight_{};

	Transform* trans_{ nullptr };

	Collider(GameObject* owner);
	virtual ~Collider() = default;

	void UpdateVertices(const glm::vec2& halfSize);

public:
};