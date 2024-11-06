#pragma once
#include <glm/vec2.hpp>

class GameObject;
class Collider;
class Transform;
class Rigidbody;

struct Manifold
{
	// 1: reference, 2: incident
	Collider* col1_;
	Collider* col2_;

	Transform* trans1_;
	Transform* trans2_;

	Rigidbody* rb1_;
	Rigidbody* rb2_;

	glm::vec2 normal_;
	double penetration_;

	int contactCnt_;
	glm::vec2 contacts_[2];

	double e_;

	Manifold(Collider* col1, Collider* col2);

	bool Solve();

	void ApplyImpulse();
	void CorrectPosition();
};