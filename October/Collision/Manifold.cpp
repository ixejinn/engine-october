#include "Manifold.h"

#include "../Component/Transform.h"
#include "../Component/Rigidbody.h"
#include "../GameObject/GameObject.h"

Manifold::Manifold(Collider* col1, Collider* col2) : col1_(col1), col2_(col2) {}
