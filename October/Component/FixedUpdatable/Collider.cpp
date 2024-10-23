#include "Collider.h"

#include "Transform.h"
#include "../../GameObject/GameObject.h"

Collider::Collider(GameObject* owner) : Component(owner)
{
	// Registry to CollisionManager

	trans_ = static_cast<Transform*>(owner_->GetComponent(typeid(Transform)));
}