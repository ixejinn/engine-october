#include "Manifold.h"

#include "../Component/Transform.h"
#include "../Component/Rigidbody.h"
#include "../Component/FixedUpdatable/Collider.h"
#include "../GameObject/GameObject.h"
#include "Collision.h"

Manifold::Manifold(Collider* col1, Collider* col2) : col1_(col1), col2_(col2)
{
    Initialize();
}

void Manifold::Initialize()
{
    if (!col1_ || !col2_)
        return;

    rb1_ = static_cast<Rigidbody*>(col1_->GetOwner()->GetComponent(typeid(Rigidbody)));
    rb2_ = static_cast<Rigidbody*>(col2_->GetOwner()->GetComponent(typeid(Rigidbody)));

    e_ = std::min(rb1_->GetRestitution(), rb2_->GetRestitution());
    
    float mass = rb1_->GetMass();
    invMass1_ = mass != 0.f ? 1 / mass : 0.f;

    mass = rb2_->GetMass();
    invMass2_ = mass != 0.f ? 1 / mass : 0.f;
}

bool Manifold::Solve()
{
    return Dispatch[col1_->GetType()][col2_->GetType()];
}

void Manifold::ApplyImpulse()
{
    if (CheckEpsilon(invMass1_ + invMass2_))
    {
        rb1_->SetVelocity({ 0.f, 0.f });
        rb2_->SetVelocity({ 0.f, 0.f });
        return;
    }


}

void Manifold::CorrectPosition()
{
}
