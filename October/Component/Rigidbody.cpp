#include "Rigidbody.h"

#include <iostream>
#include <glm/glm.hpp>
#include "Transform.h"
#include "../Manager/GameStateManager.h"
#include "../Manager/ComponentManager.h"
#include "../GameObject/GameObject.h"
#include "../Utils/imgui/imgui.h"

namespace Manager
{
	extern GameStateManager& gsMgr;
	extern ComponentManager& compMgr;
}

Rigidbody::Rigidbody(GameObject* owner) : Component(owner)
{
	trans_ = static_cast<Transform*>(owner_->GetComponent(typeid(Transform)));
}

bool Rigidbody::CheckEpsilon(float val, float epsilon)
{
	if (val < -epsilon || val > epsilon)
		return false;
	return true;
}

Rigidbody::~Rigidbody()
{
	Manager::compMgr.DeleteComponent(this);
}

void Rigidbody::Update()
{
	static const float dt = Manager::gsMgr.Step_.count() * 0.001f;

	// Linear drag
	force_ += -drag_ * velocity_;

	// Acceleration
	velocity_.x += force_.x / mass_ * dt;
	velocity_.y += force_.y / mass_ * dt;
	force_ = { 0.f, 0.f };

	if (CheckEpsilon(velocity_.x))
		velocity_.x = 0.f;
	if (CheckEpsilon(velocity_.y))
		velocity_.y = 0.f;

	if (trans_ == nullptr)	//// delete if load order is modified
		trans_ = static_cast<Transform*>(owner_->GetComponent(typeid(Transform)));
	const glm::vec2 prePos = trans_->GetPosition();

	glm::vec2 pos = prePos + velocity_ * dt;
	trans_->SetPosition(pos);
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
	ImGui::InputFloat("##mass", &mass_, 1.f, 1.0f, "%.2f");

	ImGui::Text("Drag");
	ImGui::InputFloat("##drag", &drag_, 1.f, 1.0f, "%.2f");
}

void Rigidbody::AddForce(const glm::vec2& f)
{
	force_.x += f.x;
	force_.y += f.y;
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
