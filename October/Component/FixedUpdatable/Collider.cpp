#include "Collider.h"

#include "Transform.h"
#include "../../GameObject/GameObject.h"
#include "../../Manager/CollisionManager.h"

namespace Manager
{
	extern CollisionManager& colMgr;
}

Collider::Collider(GameObject* owner) : Component(owner)
{
	updateInEditmode_ = true;

	Manager::colMgr.AddCollider(this);

	trans_ = static_cast<Transform*>(owner_->GetComponent(typeid(Transform)));
}

void Collider::UpdateVertices(const glm::vec2& halfSize)
{
	vertices_[0] = { center_.x + halfSize.x, center_.y + halfSize.y };
	vertices_[1] = { center_.x - halfSize.x, center_.y + halfSize.y };
	vertices_[2] = { center_.x - halfSize.x, center_.y - halfSize.y };
	vertices_[3] = { center_.x + halfSize.x, center_.y - halfSize.y };

	glm::mat4 transformMatrix = trans_->GetMatrix();
	for (int i = 0; i < 4; i++)
		vertices_[i] = transformMatrix * glm::vec4(vertices_[i], 0.f, 1.f);

	minVertex_ = maxVertex_ = vertices_[0];
	for (const auto& v : vertices_)
	{
		minVertex_.x = glm::min(minVertex_.x, v.x);
		minVertex_.y = glm::min(minVertex_.y, v.y);
		maxVertex_.x = glm::max(maxVertex_.x, v.x);
		maxVertex_.y = glm::max(maxVertex_.y, v.y);
	}
}
