#pragma once
#include <list>

class Collider;
class BoxCollider;

class CollisionManager
{
private:
	std::list<Collider*> colliders_{};
	
	bool CheckCollision(Collider* col1, Collider* col2);

	bool CheckAABBAABB(Collider* aabb1, Collider* aabb2);
	bool CheckAABBOBB(Collider* aabb, BoxCollider* obb);
	bool CheckOBBOBB(BoxCollider* obb1, BoxCollider* obb2);

	CollisionManager() = default;
	~CollisionManager() = default;

	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator =(const CollisionManager&) = delete;
	CollisionManager(CollisionManager&&) = delete;
	CollisionManager& operator =(CollisionManager&&) = delete;

public:
	static CollisionManager& GetInstance()
	{
		static CollisionManager instance;
		return instance;
	}

	void CheckAllCollisions();
};