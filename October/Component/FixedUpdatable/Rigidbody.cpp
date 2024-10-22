#include "Rigidbody.h"

#include "Transform.h"
#include "../../GameObject/GameObject.h"
#include "../../Utils/imgui/imgui.h"

Rigidbody::Rigidbody(GameObject* owner) : Component(owner) {}

void Rigidbody::FixedUpdate()
{
	static const float dt = FixedUpdatable::Step_.count() * 0.001f;

	force_ += -drag_ * velocity_;

	velocity_.x += force_.x / mass_ * dt;
	velocity_.y += force_.y / mass_ * dt;
	force_ = { 0.f, 0.f };

	static Transform* const trans = static_cast<Transform*>(owner_->GetComponent(typeid(Transform)));
	const glm::vec2 prePos = trans->GetPosition();

	glm::vec2 pos = prePos + velocity_ * dt;
	if (CheckEpsilon(pos.x))
		pos.x = 0;
	if (CheckEpsilon(pos.y))
		pos.y = 0;
	trans->SetPosition(pos);
}

bool Rigidbody::CheckEpsilon(float val, float epsilon)
{
	if (val < -epsilon || val > epsilon)
		return false;
	return true;
}

void Rigidbody::LoadFromJson(const json& data)
{
	auto compData = data.find("compData");
	if (compData != data.end())
	{
		auto it = compData->find("mass");
		mass_ = it.value();

		it = compData->find("drag");
		drag_ = it.value();

		it = compData->find("velocity");
		velocity_.x = it->begin().value();
		velocity_.y = (it->begin() + 1).value();
	}
}

json Rigidbody::SaveToJson()
{
	json data, compData;
	data["type"] = typeid(Rigidbody).name();

	compData["mass"] = mass_;
	compData["drag"] = drag_;
	compData["velocity"] = { velocity_.x, velocity_.y };

	data["compData"] = compData;
	return data;
}

void Rigidbody::ShowDetails()
{
	ImGui::SeparatorText("Rigidbody");

	ImGui::Text("Mass");
	ImGui::InputFloat("##mass", &mass_, 0.01f, 1.0f, "%.2f");

	ImGui::Text("Drag");
	ImGui::InputFloat("##drag", &drag_, 0.01f, 1.0f, "%.2f");
}

void Rigidbody::AddForce(const glm::vec2& f)
{
	force_ += f;
}

void Rigidbody::AddForce(const float& fx, const float& fy)
{
	force_.x += fx;
	force_.y += fy;
}

Component* Rigidbody::CreateComponent(GameObject* owner)
{
	Rigidbody* newComp = new Rigidbody(owner);
	return static_cast<Component*>(newComp);
}
