#include "Manifold.h"

#include "../Component/Transform.h"
#include "../Component/Rigidbody.h"
#include "../GameObject/GameObject.h"
#include "Collision.h"

Manifold::Manifold(Collider* col1, Collider* col2) : col1_(col1), col2_(col2)
{
    if (!col1 || !col2)
        return;

    GameObject* obj1 = col1_->GetOwner();
    GameObject* obj2 = col2_->GetOwner();

    trans1_ = static_cast<Transform*>(obj1->GetComponent(typeid(Transform)));
    trans2_ = static_cast<Transform*>(obj2->GetComponent(typeid(Transform)));

    rb1_ = static_cast<Rigidbody*>(obj1->GetComponent(typeid(Rigidbody)));
    rb2_ = static_cast<Rigidbody*>(obj2->GetComponent(typeid(Rigidbody)));

    e_ = std::min(rb1_->GetRestitution(), rb2_->GetRestitution());
}

bool Manifold::Solve()
{
    return Dispatch[col1_->GetType()][col2_->GetType()](this);
}

void Manifold::ApplyImpulse()
{
    double sumInvMass = rb1_->GetInvMass() + rb2_->GetInvMass();
    if (CheckEpsilon(sumInvMass))
    {
        rb1_->SetVelocity({ 0.f, 0.f });
        rb2_->SetVelocity({ 0.f, 0.f });
        return;
    }

    for (const auto& contact : contacts_)
    {
        glm::vec2 r1 = contact - trans1_->GetPosition();
        glm::vec2 r2 = contact - trans2_->GetPosition();

        glm::vec2 rv = rb2_->GetVelocity() - rb1_->GetVelocity();
        double contactVelocity = glm::dot(rv, normal_);
        if (contactVelocity > 0)
            return;

        double impulseScalar = -(e_ + 1) * contactVelocity;
        impulseScalar /= sumInvMass;
        impulseScalar /= contactCnt_;

        glm::vec2 impulse = normal_ * float(impulseScalar);
        rb1_->AddVelocity(-impulse);
        rb2_->AddVelocity(impulse);

        // 접선 방향 마찰력
        rv = rb2_->GetVelocity() - rb1_->GetVelocity();

        glm::vec2 tangent = glm::normalize(rv - (normal_ * glm::dot(rv, normal_)));

        double tangentImpulseScalar = -glm::dot(rv, tangent);
        tangentImpulseScalar /= sumInvMass;
        tangentImpulseScalar /= contactCnt_;

        if (CheckEpsilon(tangentImpulseScalar))
            return;

        glm::vec2 tangentImpulse = tangent * float(tangentImpulseScalar);  // 이해 안 돼서 static만... 일단..
        rb1_->AddVelocity(-tangentImpulse);
        rb2_->AddVelocity(tangentImpulse);
    }
}

void Manifold::CorrectPosition()
{
    static const double percent = 0.4;  // 0.2 ~ 0.8
    static const double slop = 0.05;    // 0.01 ~ 0.1 prevent jitter

    glm::vec2 correction = float((std::max(penetration_ - slop, 0.0) / (rb1_->GetInvMass() + rb2_->GetInvMass())) * percent) * normal_;
    trans1_->SetPosition(trans1_->GetPosition() - correction * rb1_->GetInvMass());
    trans2_->SetPosition(trans2_->GetPosition() + correction * rb2_->GetInvMass());
}
