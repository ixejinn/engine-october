#pragma once
#include <glm/vec2.hpp>

class Collider;
class Rigidbody;

struct Manifold
{
	Collider* col1_;
	Collider* col2_;

	Rigidbody* rb1_;
	Rigidbody* rb2_;

	glm::vec2 normal_;
	double penetration_;

	int contactCnt_;
	glm::vec2 contacts_[2];

	double e_;
	float invMass1_, invMass2_;

	Manifold(Collider* col1, Collider* col2);

	void Initialize();

	bool Solve();

	void ApplyImpulse();
	void CorrectPosition();
};