#include "BoxCollider.h"

#include "Transform.h"
#include "../../Utils/imgui/imgui.h"

BoxCollider::BoxCollider(GameObject* owner) : Collider(owner)
{
    scale_ = trans_->GetScale();
}

void BoxCollider::FixedUpdate()
{
    if (scaleWithTransform)
        scale_ = trans_->GetScale();
	UpdateVertices(trans_->GetLocalScale() * scale_ * 0.5f);
}

void BoxCollider::LoadFromJson(const json& data)
{
    auto compData = data.find("compData");
    if (compData != data.end())
    {
        auto it = compData->find("center");
        center_.x = it->begin().value();
        center_.y = (it->begin() + 1).value();

        it = compData->find("scale");
        scale_.x = it->begin().value();
        scale_.y = (it->begin() + 1).value();
    }
}

json BoxCollider::SaveToJson()
{
    json data, compData;
    data["type"] = typeid(Transform).name();

    compData["center"] = { center_.x, center_.y };
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
    ImGui::Checkbox("Scale with transform", &scaleWithTransform);
    if (!scaleWithTransform)
    {
        ImGui::DragFloat("x##boxCollider_scale", &scale_[0], 0.01f);
        ImGui::DragFloat("y##boxCollider_scale", &scale_[1], 0.01f);
    }
}

Component* BoxCollider::CreateComponent(GameObject* owner)
{
	BoxCollider* newComp = new BoxCollider(owner);
	return static_cast<BoxCollider*>(newComp);
}
