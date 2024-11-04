#include "Manifold.h"

#include "../Component/Transform.h"
#include "../Component/Rigidbody.h"
#include "../GameObject/GameObject.h"

Manifold::Manifold(GameObject* obj1, GameObject* obj2)
{
	tf1_ = static_cast<Transform*>(obj1->GetComponent(typeid(Transform)));
	tf2_ = static_cast<Transform*>(obj2->GetComponent(typeid(Transform)));

	rb1_ = static_cast<Rigidbody*>(obj1->GetComponent(typeid(Rigidbody)));
	rb2_ = static_cast<Rigidbody*>(obj2->GetComponent(typeid(Rigidbody)));
}
