#include "CollisionManager.h"

#include <deque>
#include "../Component/FixedUpdatable/BoxCollider.h"
#include "../Component/FixedUpdatable/CircleCollider.h"
#include "../Component/Transform.h"
#include "../Component/Collidable.h"
#include "../GameObject/GameObject.h"
#include "../Collision/Collision.h"
#include "../Collision/Manifold.h"
#include "../Profiler/Profiler.h"

CollisionManager::CollisionManager()
{
	layerCollisionMatrix_[0] |= 1 << 31;	// default layer
}

void CollisionManager::CheckAllCollisions()
{
	static std::deque<Manifold> manifolds{};

	for (auto it1 = colliders_.begin(); it1 != colliders_.end(); ++it1)
	{
		auto it2 = std::next(it1);

		for (; it2 != colliders_.end(); ++it2)
		{
			Manifold manifold(*it1, *it2);

			if (
				(*it1)->owner_ != (*it2)->owner_ &&
				(*it1)->owner_->active_ && (*it2)->owner_->active_ &&
				CheckLayerCollisionMatrix(*it1, *it2) &&
				AABBtoAABB(*it1, *it2) &&
				manifold.Solve()
				)
				manifolds.emplace_back(manifold);
		}
	}

	while (!manifolds.empty())
	{
		Manifold& manifold = manifolds.front();

		manifold.ApplyImpulse();
		manifold.CorrectPosition();

		manifolds.pop_front();
	}
}

void CollisionManager::AddCollider(Collider* col)
{
	colliders_.emplace_back(col);
}

void CollisionManager::Clear()
{
	colliders_.clear();
}

bool CollisionManager::CheckLayerCollisionMatrix(Collider* col1, Collider* col2) const
{
	return layerCollisionMatrix_[col1->layer_] & 1 << (31 - col2->layer_);
}

bool CollisionManager::AABBtoAABB(Collider* aabb1, Collider* aabb2)
{
	if (aabb1->maxVertex_.x < aabb2->minVertex_.x || aabb1->minVertex_.x > aabb2->maxVertex_.x) return false;
	if (aabb1->maxVertex_.y < aabb2->minVertex_.y || aabb1->minVertex_.y > aabb2->maxVertex_.y) return false;
	return true;
}

bool CollisionManager::CheckAABBOBB(Collider* aabb, BoxCollider* obb)
{
	return CheckOBBOBB(static_cast<BoxCollider*>(aabb), obb);
}

bool CollisionManager::CheckAABBCircle(Collider* aabb, CircleCollider* circle)
{
	glm::vec2 colliderCtrCircle = circle->trans_->GetPosition() + circle->center_;
	float radius = circle->radius_;

	glm::vec2 colliderRangeAABB[2] = { aabb->minVertex_, aabb->maxVertex_ };

	// AABB�� circle collider�� ��������ŭ Ȯ�����
	// circle collider�� center�� AABB�� ���ο� �ְų� ��ġ�ϴ� ���� Ȯ���Ͽ� �浹 ���� ����
	// 
	// ����: Ȯ��� AABB�� �𼭸� ��ó(�밢�� ����)�� ���� �������� ���, �浹���� ������ ���� ���ǿ����� true�� ��
	//      (Ȯ��� AABB�� �𼭸��� ���� ���������� ������� ����)

	// Circle collider�� AABB�� �밢�� ������ �ƴ� ���� ��ġ�ϴ� ���
	if (
		(colliderCtrCircle.x >= colliderRangeAABB[0].x && colliderCtrCircle.x <= colliderRangeAABB[1].x) ||
		(colliderCtrCircle.y >= colliderRangeAABB[0].y && colliderCtrCircle.y <= colliderRangeAABB[1].y)
		)
	{
		// AABB�� circle collider�� ��������ŭ Ȯ������� ��
		colliderRangeAABB[0] -= radius;
		colliderRangeAABB[1] += radius;

		// Circle collider�� center�� AABB�� ���ο� �ְų� ��ġ�ϸ� �浹 ����
		if (
			colliderCtrCircle.x >= colliderRangeAABB[0].x &&
			colliderCtrCircle.x <= colliderRangeAABB[1].x &&
			colliderCtrCircle.y >= colliderRangeAABB[0].y &&
			colliderCtrCircle.y <= colliderRangeAABB[1].y
			)
			return true;
	}
	// ����
	else
	{
		glm::vec2 verticesAABB[4]
		{
			aabb->maxVertex_,
			{ aabb->minVertex_.x, aabb->maxVertex_.y },
			aabb->minVertex_,
			{ aabb->maxVertex_.x, aabb->minVertex_.y }
		};

		for (int i = 0; i < 4; i++)
		{
			// 4�� ������ circle collider center���� �Ÿ��� �������� ���ų� ���� ��찡 �ϳ��� ������ �浹 ����
			if (glm::distance(verticesAABB[i], colliderCtrCircle) <= radius)
				return true;
		}
	}

	return false;
}

bool CollisionManager::CheckOBBOBB(BoxCollider* obb1, BoxCollider* obb2)
{
	// Separating axis theorem
	// �� ��ü�� �������� �� ��ġ�� �ʴ� ���� �ϳ��� �ִٸ� �浹���� ����
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
		// minDotProduct�� 0���� ũ�ٸ� normal ���Ϳ� �����̰� inext�� ������ ����(�и���)�� ��������
		// obb2�� ��� ������ obb1�� �ܺο� �ִٴ� ���� ��
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
	float sumRadius = circle1->radius_ + circle2->radius_;
	glm::vec2 diff = (circle1->trans_->GetPosition() + circle1->center_) - (circle2->trans_->GetPosition() + circle2->center_);

	float squaredDist = diff.x * diff.x + diff.y * diff.y;
	return sumRadius * sumRadius >= squaredDist;
}
