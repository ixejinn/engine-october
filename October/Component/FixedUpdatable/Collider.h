#pragma once
#include <glm/vec2.hpp>
#include <map>
#include <string>

#include "../Component.h"
#include "FixedUpdatable.h"
#include "../../Utils/Utils.h"

class Transform;

class Collider : public Component, public FixedUpdatable
{
public:
	enum ColliderType
	{
		CONVEX,
		CIRCLE,
		ColliderTypeCnt
	};

protected:
	ColliderType type_{ CONVEX };
	std::map<ColliderType, std::string> typeToString_{
		{ CONVEX, "CONVEX" },
		{ CIRCLE, "CIRCLE" }
	};
	
	int layer_ = 0;	// 수정 필요.. 아마 enum

	glm::vec2 center_{ 0.f, 0.f };

	bool scaleWithTransform_{ true };
	glm::vec2 vertices_[4]{};
	glm::vec2 minVertex_{};
	glm::vec2 maxVertex_{};

	Transform* trans_{ nullptr };

	Collider(GameObject* owner);
	virtual ~Collider() = default;

	void UpdateVertices(const glm::vec2& halfSize);

public:
	virtual void SetScaleWithTransform(bool) = 0;

	friend class CollisionManager;
};