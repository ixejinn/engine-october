#include "BoxCollider.h"

#include "Transform.h"

BoxCollider::BoxCollider(GameObject* owner) : Collider(owner) {}

void BoxCollider::FixedUpdate()
{
	glm::vec2 halfSize = trans_->GetLocalScale() * scale_ * 0.5f;
	vertices_[0] = { center_.x + halfSize.x, center_.y + halfSize.y };
	vertices_[1] = { center_.x - halfSize.x, center_.y + halfSize.y };
	vertices_[2] = { center_.x - halfSize.x, center_.y - halfSize.y };
	vertices_[3] = { center_.x + halfSize.x, center_.y - halfSize.y };

	glm::mat4 transformMatrix = trans_->GetMatrix();
	//for (int i = 0; i < 4; i++)
	//	vertices_[i] = 
}

void BoxCollider::LoadFromJson(const json& data)
{
}

json BoxCollider::SaveToJson()
{
	return json();
}

void BoxCollider::ShowDetails()
{
}

Component* BoxCollider::CreateComponent(GameObject* owner)
{
	return nullptr;
}
