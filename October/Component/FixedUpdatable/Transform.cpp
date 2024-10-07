#include "Transform.h"

#include <iostream>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(GameObject* owner) : Component(owner), transformMatrix_() {}

void Transform::UpdateMatrix()
{
    transformMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(position_.x, position_.y, 0.f));
    transformMatrix_ = glm::rotate(transformMatrix_, glm::radians(rotation_), glm::vec3(0.f, 0.f, 1.f));
    transformMatrix_ = glm::scale(transformMatrix_, glm::vec3(scale_.x, scale_.y, 0.f));
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
