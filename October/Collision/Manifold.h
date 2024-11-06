#pragma once
#include <glm/vec2.hpp>

class Collider;

struct Manifold
{
	Collider* col1_;
	Collider* col2_;

	glm::vec2 normal_;
	double penetration_;

	int contactCnt_;
	glm::vec2 contacts_[2];

	double e_;

	Manifold(Collider* col1, Collider* col2);
};