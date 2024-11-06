#pragma once
#include <list>

class Collider;
class BoxCollider;
class CircleCollider;

class CollisionManager
{
private:
	std::list<Collider*> colliders_{};

	unsigned int layerCollisionMatrix_[32]{ 0, };
	bool CheckLayerCollisionMatrix(Collider* col1, Collider* col2) const;

	bool AABBtoAABB(Collider* aabb1, Collider* aabb2);

	bool CheckAABBOBB(Collider* aabb, BoxCollider* obb);
	bool CheckAABBCircle(Collider* aabb, CircleCollider* circle);
	bool CheckOBBOBB(BoxCollider* obb1, BoxCollider* obb2);
	bool CheckOBB(BoxCollider* obb1, BoxCollider* obb2);
	bool CheckOBBCircle(BoxCollider* obb, CircleCollider* circle);
	bool CheckCircleCircle(CircleCollider* circle1, CircleCollider* circle2);

	CollisionManager();
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

	void AddCollider(Collider* col);

	void Clear();
};