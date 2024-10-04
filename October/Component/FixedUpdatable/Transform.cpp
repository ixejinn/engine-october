#include "Transform.h"

#include <iostream>
#include <glm/trigonometric.hpp>

Transform::Transform(GameObject* owner) : Component(owner), transformMatrix_() {}

void Transform::UpdateMatrix()
{
    auto rotationRad = glm::radians(rotation_);

    transformMatrix_ = glm::mat3(
        scale_.x * glm::cos(rotationRad), glm::sin(rotationRad), 0,
        -glm::sin(rotationRad), scale_.y * glm::cos(rotationRad), 0,
        position_.x, position_.y, 1
    );
}

void Transform::FixedUpdate()
{
    UpdateMatrix();
}

void Transform::SetPosition(const glm::vec2& pos)
{
    position_ = pos;
    UpdateMatrix();
}

void Transform::SetPosition(const float& x, const float& y)
{
    position_.x = x;
    position_.y = y;
    UpdateMatrix();
}

void Transform::SetScale(const glm::vec2& scl)
{
    scale_ = scl;
    UpdateMatrix();
}

void Transform::SetLocalScale(const glm::vec2& scl)
{
    localScale_ = scl;
}

void Transform::SetRotation(const glm::vec2& rot)
{
    SetRotation(glm::atan(rot.y, rot.x));
}

void Transform::SetRotation(const float& rot)
{
    rotation_ = rot;
    UpdateMatrix();
}

void Transform::SetFlip()
{
    scale_.x *= -1;
    UpdateMatrix();
}

Component* Transform::CreateComponent(GameObject* owner)
{
    Transform* newComp = new Transform(owner);
    return static_cast<Component*>(newComp);
}
