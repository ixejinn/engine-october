#include "CircleCollider.h"

#include "../Transform.h"
#include "../../Utils/imgui/imgui.h"

CircleCollider::CircleCollider(GameObject* owner) : Collider(owner)
{
	type_ = CIRCLE;
}

void CircleCollider::FixedUpdate()
{
	UpdateVertices(trans_->GetLocalScale() * radius_);
}

void CircleCollider::LoadFromJson(const json& data)
{
	auto compData = data.find("compData");
	if (compData != data.end())
	{
		auto it = compData->find("layer");
		layer_ = it->begin().value();

		it = compData->find("trigger");
		trigger_ = it->begin().value();

		it = compData->find("center");
		center_.x = it->begin().value();
		center_.y = (it->begin() + 1).value();

		it = compData->find("scaleWithTransform");
		scaleWithTransform_ = it->begin().value();

		if (!scaleWithTransform_)
		{
			it = compData->find("radius");
			radius_ = it->begin().value();
		}
	}
}

json CircleCollider::SaveToJson()
{
	json data, compData;
	data["type"] = typeid(CircleCollider).name();

	compData["layer"] = layer_;
	compData["trigger"] = trigger_;
	compData["center"] = { center_.x, center_.y };
	compData["scaleWithTransform"] = scaleWithTransform_;
	if (!scaleWithTransform_)
		compData["radius"] = radius_;

	data["compData"] = compData;
	return data;
}

void CircleCollider::ShowDetails()
{
	ImGui::SeparatorText("CircleCollider");

	ImGui::Text("Trigger");
	ImGui::Checkbox("##circleCollider_trigger", &trigger_);

	ImGui::Text("Center");
	ImGui::InputFloat2("##circleCollider_center", &center_[0]);

	ImGui::Text("Radius");
	ImGui::Checkbox("Radius with transform", &scaleWithTransform_);
	if (scaleWithTransform_)
		SetRadius(true);
	else
		ImGui::DragFloat("x##circleCollider_radius", &radius_, 0.01f);
}

void CircleCollider::SetRadius(bool withTransform, float radius)
{
	scaleWithTransform_ = withTransform;

	radius_ = radius;
}

Component* CircleCollider::CreateComponent(GameObject* owner)
{
	CircleCollider* newComp = new CircleCollider(owner);
	return static_cast<CircleCollider*>(newComp);
}
