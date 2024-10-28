#include "CollisionManager.h"

#include <queue>
#include "../Component/FixedUpdatable/BoxCollider.h"
#include "../Component/FixedUpdatable/CircleCollider.h"
#include "../Component/FixedUpdatable/Transform.h"
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
			return CheckAABBCircle(colA, static_cast<CircleCollider*>(colB));
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
			return CheckOBBCircle(static_cast<BoxCollider*>(colA), static_cast<CircleCollider*>(colB));
		}
		break;
	}

	case Collider::CIRCLE:
	{
		return CheckCircleCircle(static_cast<CircleCollider*>(colA), static_cast<CircleCollider*>(colB));
		break;
	}

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

bool CollisionManager::CheckAABBCircle(Collider* aabb, CircleCollider* circle)
{
	glm::vec2 colliderCtrCircle = circle->trans_->GetPosition() + circle->center_;
	float radius = circle->radius_;

	glm::vec2 colliderRangeAABB[2] = { aabb->bottomLeft_, aabb->topRight_ };

	// AABB를 circle collider의 반지름만큼 확장시켜
	// circle collider의 center가 AABB의 내부에 있거나 일치하는 지를 확인하여 충돌 감지 가능
	// 
	// 예외: 확장된 AABB의 모서리 근처(대각선 방향)에 원이 걸쳐지는 경우, 충돌하지 않으나 위의 조건에서는 true가 됨
	//      (확장된 AABB의 모서리는 원의 반지름보다 길어지기 때문)

	// Circle collider가 AABB의 대각선 방향이 아닌 곳에 위치하는 경우
	//if (
	//	(colliderCtrCircle.x >= colliderRangeAABB[0].x && colliderCtrCircle.x <= colliderRangeAABB[1].x) ||
	//	(colliderCtrCircle.y >= colliderRangeAABB[0].y && colliderCtrCircle.y <= colliderRangeAABB[1].y)
	//	)
	{
		// AABB를 circle collider의 반지름만큼 확장시켰을 때
		colliderRangeAABB[0] -= radius;
		colliderRangeAABB[1] += radius;

		// Circle collider의 center가 AABB의 내부에 있거나 일치하면 충돌 있음
		if (
			colliderCtrCircle.x >= colliderRangeAABB[0].x &&
			colliderCtrCircle.x <= colliderRangeAABB[1].x &&
			colliderCtrCircle.y >= colliderRangeAABB[0].y &&
			colliderCtrCircle.y <= colliderRangeAABB[1].y
			)
			return true;
	}
	// 예외
	//else
	//{
	//	glm::vec2 verticesAABB[4]
	//	{
	//		aabb->topRight_,
	//		{ aabb->bottomLeft_.x, aabb->topRight_.y },
	//		aabb->bottomLeft_,
	//		{ aabb->topRight_.x, aabb->bottomLeft_.y }
	//	};

	//	for (int i = 0; i < 4; i++)
	//	{
	//		// 4개 정점과 circle collider center와의 거리가 반지름과 같거나 작으면 충돌 있음
	//		auto test = glm::length(verticesAABB[i] - colliderCtrCircle);
	//		if (glm::distance(verticesAABB[i], colliderCtrCircle) <= radius)
	//			return true;
	//	}
	//}

	return false;
}

bool CollisionManager::CheckOBBOBB(BoxCollider* obb1, BoxCollider* obb2)
{
	// Separating axis theorem
	// 두 물체를 투영했을 때 겹치지 않는 축이 하나라도 있다면 충돌하지 않음
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
		// minDotProduct가 0보다 크다면 normal 벡터와 수직이고 inext를 지나는 직선(분리축)을 기준으로
		// obb2의 모든 정점이 obb1의 외부에 있다는 것이 됨
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

bool CollisionManager::CheckOBBCircle(BoxCollider* obb, CircleCollider* circle)
{
	return false;
}

bool CollisionManager::CheckCircleCircle(CircleCollider* circle1, CircleCollider* circle2)
{
	return false;
}
