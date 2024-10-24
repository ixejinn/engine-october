#include "CollisionManager.h"

#include <queue>
#include "../Component/FixedUpdatable/BoxCollider.h"

void CollisionManager::CheckAllCollisions()
{
	static std::queue<std::pair<Collider*, Collider*>> collisionPairs{};

	for (auto it1 = colliders_.begin(); it1 != colliders_.end(); ++it1)
	{
		auto it2 = std::next(it1);
		for (; it2 != colliders_.end(); ++it2)
		{
			if ((*it1)->owner_ != (*it2)->owner_ && CheckCollision(*it1, *it2))
				collisionPairs.push({ *it1, *it2 });
		}
	}

	while (!collisionPairs.empty())
	{

	}
}

bool CollisionManager::CheckCollision(Collider* col1, Collider* col2)
{
	return false;
}

bool CollisionManager::CheckAABBAABB(Collider* aabb1, Collider* aabb2)
{
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
