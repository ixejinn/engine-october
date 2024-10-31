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

	bottomLeft_ = topRight_ = vertices_[0];
	for (const auto& v : vertices_)
	{
		bottomLeft_.x = glm::min(bottomLeft_.x, v.x);
		bottomLeft_.y = glm::min(bottomLeft_.y, v.y);
		topRight_.x = glm::max(topRight_.x, v.x);
		topRight_.y = glm::max(topRight_.y, v.y);
	}
}
