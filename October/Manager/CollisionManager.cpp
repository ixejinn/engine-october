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

	return true;	// 나중에 수정하기
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
	return false;
}

bool CollisionManager::CheckOBBOBB(BoxCollider* obb1, BoxCollider* obb2)
{
	return false;
}
