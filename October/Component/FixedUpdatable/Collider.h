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
		AABB,
		OBB,
		CIRCLE,
		ColliderTypeCnt
	};

protected:
	ColliderType type_{ AABB };
	std::map<ColliderType, std::string> typeToString_{
		{ AABB, "AABB" },
		{ OBB, "OBB" },
		{ CIRCLE, "CIRCLE" }
	};
	
	int layer_{ 0 };	// 수정 필요.. 아마 enum
	bool trigger_{ true };

	glm::vec2 center_{ 0.f, 0.f };

	bool scaleWithTransform_{ true };
	glm::vec2 vertices_[4]{};
	glm::vec2 minVertex_{};
	glm::vec2 maxVertex_{};

	Transform* trans_{ nullptr };

	Collider(GameObject* owner);
	virtual ~Collider();

	void UpdateVertices(const glm::vec2& halfSize);

public:
	void SetTrigger(bool b);

	friend class CollisionManager;
};