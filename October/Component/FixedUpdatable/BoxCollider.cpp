#include "BoxCollider.h"

#include "../Transform.h"
#include "../../Utils/imgui/imgui.h"

BoxCollider::BoxCollider(GameObject* owner) : Collider(owner)
{
    type_ = CONVEX;

    normals_[0] = { 0.f, 1.f };
    normals_[1] = { -1.f, 0.f };
    normals_[2] = { 0.f, -1.f };
    normals_[3] = { 1.f, 0.f };

    scale_ = trans_->GetScale();
}

void BoxCollider::FixedUpdate()
{
	UpdateVertices(trans_->GetLocalScale() * scale_ * 0.5f);

    glm::mat4 transformMatrix = trans_->GetMatrix();
    for (int i = 0; i < 4; i++)
        normals_[i] = glm::normalize(transformMatrix * glm::vec4(normals_[i], 0.f, 1.f));
}

void BoxCollider::LoadFromJson(const json& data)
{
    auto compData = data.find("compData");
    if (compData != data.end())
    {
        auto it = compData->find("type");
        type_ = it->begin().value();

        it = compData->find("layer");
        layer_ = it->begin().value();

        it = compData->find("center");
        center_.x = it->begin().value();
        center_.y = (it->begin() + 1).value();

        it = compData->find("scaleWithTransform");
        scaleWithTransform_ = it->begin().value();

        it = compData->find("scale");
        scale_.x = it->begin().value();
        scale_.y = (it->begin() + 1).value();
    }
}

json BoxCollider::SaveToJson()
{
    json data, compData;
    data["type"] = typeid(BoxCollider).name();

    compData["type"] = type_;
    compData["layer"] = layer_;
    compData["center"] = { center_.x, center_.y };
    compData["scaleWithTransform"] = scaleWithTransform_;
    compData["scale"] = { scale_.x, scale_.y };

    data["compData"] = compData;
    return data;
}

void BoxCollider::ShowDetails()
{
    ImGui::SeparatorText("BoxCollider");

    ImGui::Text("Center");
    ImGui::InputFloat2("##boxCollider_center", &center_[0]);

    ImGui::Text("Scale");
    ImGui::Checkbox("Scale with transform", &scaleWithTransform_);
    if (!scaleWithTransform_)
    {
        ImGui::DragFloat("x##boxCollider_scale", &scale_[0], 0.01f);
        ImGui::DragFloat("y##boxCollider_scale", &scale_[1], 0.01f);
    }
}

void BoxCollider::SetScaleWithTransform(bool b)
{
    scaleWithTransform_ = b;

    if (b)
        scale_.x = scale_.y = 1.f;
}

Component* BoxCollider::CreateComponent(GameObject* owner)
{
	BoxCollider* newComp = new BoxCollider(owner);
	return static_cast<BoxCollider*>(newComp);
}
