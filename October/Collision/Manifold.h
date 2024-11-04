#pragma once
#include <glm/vec2.hpp>

class Transform;
class Rigidbody;
class GameObject;

struct Manifold
{
	Transform* tf1_;
	Transform* tf2_;

	Rigidbody* rb1_;
	Rigidbody* rb2_;

	glm::vec2 normal_;
	double penetration_;

	int contactCnt_;
	glm::vec2 contacts_[2];

	double e_;

	Manifold(GameObject* obj1, GameObject* obj2);
};