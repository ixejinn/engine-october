#include "PlayerController.h"

#include <glm/glm.hpp>
#include "../../GameObject/GameObject.h"
#include "../FixedUpdatable/Rigidbody.h"
#include "../../Utils/Setting.h"
#include "../../Utils/Utils.h"
#include "../../Utils/imgui/imgui.h"

PlayerController::PlayerController(GameObject* owner) : Component(owner) {}

void PlayerController::Update()
{
	static Rigidbody* rb = static_cast<Rigidbody*>(owner_->GetComponent(typeid(Rigidbody)));

	glm::vec2 moveVec{ 0.f, 0.f };

	//if (horizontal_)
	//{
	//	if (glfwGetKey(window, moveKeys_[LEFT]) == GLFW_PRESS)
	//		moveVec.x--;

	//	if (glfwGetKey(window, moveKeys_[RIGHT]) == GLFW_PRESS)
	//		moveVec.x++;
	//}

	//if (vertical_)
	//{
	//	if (glfwGetKey(window, moveKeys_[UP]) == GLFW_PRESS)
	//		moveVec.y--;

	//	if (glfwGetKey(window, moveKeys_[DOWN]) == GLFW_PRESS)
	//		moveVec.y++;
	//}

	if (glm::length(moveVec) > 1.f)
		moveVec = glm::normalize(moveVec);
	rb->AddForce(moveVec * speed_);
}

void PlayerController::LoadFromJson(const json& data)
{
	auto compData = data.find("compData");
	if (compData != data.end())
	{
		auto it = compData->find("vertical");
		vertical_ = it.value();

		it = compData->find("horizontal");
		horizontal_ = it.value();

		it = compData->find("moveKeys");
		moveKeys_[LEFT] = it->begin().value();
		moveKeys_[RIGHT] = (it->begin() + 1).value();
		moveKeys_[UP] = (it->begin() + 2).value();
		moveKeys_[DOWN] = (it->begin() + 3).value();

		it = compData->find("speed");
		speed_ = it.value();
	}
}

json PlayerController::SaveToJson()
{
	json data, compData;
	data["type"] = typeid(PlayerController).name();

	compData["vertical"] = vertical_;
	compData["horizontal"] = horizontal_;
	compData["moveKeys"] = { moveKeys_[LEFT], moveKeys_[RIGHT], moveKeys_[UP], moveKeys_[DOWN] };
	compData["speed"] = speed_;

	data["compData"] = compData;
	return data;
}

void PlayerController::ShowDetails()
{
	ImGui::SeparatorText("PlayerController");

	ImGui::Checkbox("Vertical", &vertical_);
	ImGui::Checkbox("Horizontal", &horizontal_);

	ImGui::Text("Speed");
	ImGui::InputFloat("##speed", &speed_, 0.01f, 1.0f, "%.2f");
}

Component* PlayerController::CreateComponent(GameObject* owner)
{
	PlayerController* newComp = new PlayerController(owner);
	return static_cast<Component*>(newComp);
}
