#include "CollisionManager.h"

#include <queue>
#include "../Component/FixedUpdatable/BoxCollider.h"
#include "../Component/Collidable.h"
#include "../GameObject/GameObject.h"
#include "../Collision/Collision.h"

CollisionManager::CollisionManager()
{
	layerCollisionMatrix_[0] |= 1 << 31;	// default setting for layer collision
}

void CollisionManager::CheckAllCollisions()
{
	static std::queue<std::pair<Collider*, Collider*>> colliderPairs{};

	for (auto it1 = colliders_.begin(); it1 != colliders_.end(); ++it1)
	{
		auto it2 = std::next(it1);
		for (; it2 != colliders_.end(); ++it2)
		{
			if ((*it1)->owner_->active_ && (*it2)->owner_->active_ &&
				CheckCollision(*it1, *it2))
				colliderPairs.push({ *it1, *it2 });
		}
	}

	while (!colliderPairs.empty())
	{
		std::pair<Collider*, Collider*> colliderPair = colliderPairs.front();

		Collidable* first = nullptr;
		Collidable* second = nullptr;

		if (first = colliderPair.first->owner_->GetCollidable())
		{
			Collision* collision1 = new Collision();
			first->OnCollision(collision1);
			delete collision1;
		}

		if (second = colliderPair.second->owner_->GetCollidable())
		{
			Collision* collision2 = new Collision();
			second->OnCollision(collision2);
			delete collision2;
		}

		colliderPairs.pop();
	}
}

void CollisionManager::AddCollider(Collider* col)
{
	colliders_.emplace_back(col);
}

void CollisionManager::Clear()
{
	for (auto it = colliders_.begin(); it != colliders_.end(); )
		colliders_.erase(it++);
}

bool CollisionManager::CheckCollision(Collider* col1, Collider* col2)
{
	if (col1->owner_ == col2->owner_)
		return false;

	if (!(layerCollisionMatrix_[col1->layer_] & 1 << (31 - col2->layer_)))
		return false;

	if (!CheckAABBAABB(col1, col2))
		return false;

	if (col1->type_ == Collider::AABB && col2->type_ == Collider::AABB)
		return true;

	Collider* colA = col1;
	Collider* colB = col2;
	if (col1->type_ > col2->type_)
	{
		colA = col2;
		colB = col1;
	}

	switch (colA->type_)
	{
	case Collider::AABB:
	{
		switch (colB->type_)
		{
		case Collider::OBB:
			return CheckAABBOBB(colA, static_cast<BoxCollider*>(colB));

		case Collider::CIRCLE:
			return false;
		}
		break;
	}

	case Collider::OBB:
	{
		switch (colB->type_)
		{
		case Collider::OBB:
			return CheckOBBOBB(static_cast<BoxCollider*>(colA), static_cast<BoxCollider*>(colB));

		case Collider::CIRCLE:
			return false;
		}
		break;
	}

	case Collider::CIRCLE:
		return false;
		break;

	default:
		return false;
	}
}

bool CollisionManager::CheckAABBAABB(Collider* aabb1, Collider* aabb2)
{
	if (aabb1->topRight_.x > aabb2->bottomLeft_.x &&
		aabb1->bottomLeft_.x < aabb2->topRight_.x &&
		aabb1->topRight_.y > aabb2->bottomLeft_.y &&
		aabb1->bottomLeft_.y < aabb2->topRight_.y)
		return true;
	else
		return false;
}

bool CollisionManager::CheckAABBOBB(Collider* aabb, BoxCollider* obb)
{
	return CheckOBBOBB(static_cast<BoxCollider*>(aabb), obb);
}

bool CollisionManager::CheckOBBOBB(BoxCollider* obb1, BoxCollider* obb2)
{
	return CheckOBB(obb1, obb2) && CheckOBB(obb2, obb1);
}

bool CollisionManager::CheckOBB(BoxCollider* obb1, BoxCollider* obb2)
{
	float minDotProducts[4]{ 0.f, };
	float minDotProduct, maxDotProduct;

	// Check obb1's normal vector
	for (int i = 0; i < 4; i++)
	{
		int inext = i + 1;
		if (inext == 4)
			inext = 0;

		glm::vec2 normal
		{
			obb1->vertices_[inext].x - obb1->vertices_[i].x,
			obb1->vertices_[inext].y - obb1->vertices_[i].y
		};

		minDotProduct = FLT_MAX;
		for (const glm::vec2& v2 : obb2->vertices_)
		{
			glm::vec2 vec12{ v2.x - obb1->vertices_[inext].x, v2.y - obb1->vertices_[inext].y };

			float dotProduct = normal.x * vec12.x + normal.y * vec12.y;
			if (minDotProduct > dotProduct)
				minDotProduct = dotProduct;
		}

		minDotProducts[inext] = minDotProduct;
	}

	maxDotProduct = minDotProducts[0];
	for (int i = 1; i < 4; i++)
	{
		if (maxDotProduct < minDotProducts[i])
			maxDotProduct = minDotProducts[i];
	}

	if (maxDotProduct > 0)
		return false;
	else
		return true;
}
