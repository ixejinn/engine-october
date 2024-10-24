#pragma once
#include <glm/vec2.hpp>
#include <map>
#include <string>

#include "../Component.h"
#include "FixedUpdatable.h"

class Transform;

class Collider : public Component, public FixedUpdatable
{
public:
	enum ColliderType
	{
		AABB,
		OBB,
		CIRCLE
	};

protected:
	ColliderType type_{ AABB };
	std::map<ColliderType, std::string> typeToString_{
		{ AABB, "AABB" },
		{ OBB, "OBB" },
		{ CIRCLE, "CIRCLE" }
	};

	glm::vec2 center_{ 0.f, 0.f };

	bool scaleWithTransform{ true };
	glm::vec2 vertices_[4]{};
	glm::vec2 bottomLeft_{};
	glm::vec2 topRight_{};

	Transform* trans_{ nullptr };

	Collider(GameObject* owner);
	virtual ~Collider() = default;

	void UpdateVertices(const glm::vec2& halfSize);

public:
	void SetScaleWithTransform(bool b) { scaleWithTransform = b; }
};