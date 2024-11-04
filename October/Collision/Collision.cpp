#include "Collision.h"

CollisionCallback Dispatch[Collider::ColliderTypeCnt][Collider::ColliderTypeCnt] =
{
	{ AABBtoAABB, AABBtoOBB, AABBtoCircle },
	{ OBBtoAABB, OBBtoOBB, OBBtoCircle },
	{ CircletoAABB, CircletoOBB, CircletoCircle }
};