#include "Transform.h"

#include <iostream>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Utils/imgui/imgui.h"
#include "../Manager/ComponentManager.h"

namespace Manager
{
    extern ComponentManager& compMgr;
}

Transform::Transform(GameObject* owner) : Component(owner) {}

Transform::~Transform()
{
    Manager::compMgr.DeleteComponent(this);
}

void Transform::UpdateMatrix()
{
    transformMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(position_.x, position_.y, 0.f));
    transformMatrix_ = glm::rotate(transformMatrix_, glm::radians(rotation_), glm::vec3(0.f, 0.f, 1.f));
    transformMatrix_ = glm::scale(transformMatrix_, glm::vec3(localScale_.x * scale_.x, localScale_.y * scale_.y, 0.f));
}

void Transform::LoadFromJson(const json& data)
{
    auto compData = data.find("compData");
    if (compData != data.end())
    {
        auto it = compData->find("position");
        position_.x = it->begin().value();
        position_.y = (it->begin() + 1).value();

        it = compData->find("rotation");
        rotation_ = it.value();

        it = compData->find("scale");
        scale_.x = it->begin().value();
        scale_.y = (it->begin() + 1).value();

        it = compData->find("localScale");
        localScale_.x = it->begin().value();
        localScale_.y = (it->begin() + 1).value();
    }

    UpdateMatrix();
}

json Transform::SaveToJson()
{
    json data, compData;
    data["type"] = typeid(Transform).name();

    compData["position"] = { position_.x, position_.y };
    compData["rotation"] = rotation_;
    compData["scale"] = { scale_.x, scale_.y };
    compData["localScale"] = { localScale_.x, localScale_.y };

    data["compData"] = compData;
    return data;
}

void Transform::ShowDetails()
{
    ImGui::SeparatorText("Transform");

    ImGui::Text("Position");
    ImGui::DragFloat("x##transform_position", &position_[0], 0.01f);
    ImGui::DragFloat("y##transform_position", &position_[1], 0.01f);

    ImGui::Text("Rotation");
    ImGui::DragFloat("##transform_rotation", &rotation_, 1.f, -360.f, 360.f);

    ImGui::Text("Scale");
    ImGui::DragFloat("x##transform_scale", &scale_[0], 0.01f);
    ImGui::DragFloat("y##transform_scale", &scale_[1], 0.01f);

    //ImGui::Text("Local Scale");
    //ImGui::DragFloat("local scale x", &localScale_[0], 0.01f);
    //ImGui::DragFloat("local scale y", &localScale_[1], 0.01f);
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

void Transform::AddRotation(const float& rot)
{
    rotation_ += rot;
}

Component* Transform::CreateComponent(GameObject* owner)
{
    Transform* newComp = new Transform(owner);
    return static_cast<Component*>(newComp);
}
