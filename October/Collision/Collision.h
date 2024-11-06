#pragma once
#include "../Component/FixedUpdatable/Collider.h"

struct Manifold;

typedef bool (*CollisionCallback)(Manifold* m);

extern CollisionCallback Dispatch[Collider::ColliderTypeCnt][Collider::ColliderTypeCnt];

bool ConvextoConvex(Manifold* m);
bool ConvextoCircle(Manifold* m);
bool CircletoCircle(Manifold* m);

struct Collision
{

};