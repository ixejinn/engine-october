#include "PlayerController.h"

#include <glm/glm.hpp>
#include "../Rigidbody.h"
#include "../../GameObject/GameObject.h"
#include "../../Manager/SettingManager.h"
#include "../../Utils/imgui/imgui.h"

PlayerController::PlayerController(GameObject* owner) : Component(owner)
{
	rb_ = static_cast<Rigidbody*>(owner_->GetComponent(typeid(Rigidbody)));
}

void PlayerController::Update()
{
	if (rb_ == nullptr)
		rb_ = static_cast<Rigidbody*>(owner_->GetComponent(typeid(Rigidbody)));

	GLFWwindow* window = SettingManager::GetInstance().GetWindow();

	glm::vec2 moveVec{ 0.f, 0.f };

	if (horizontal_)
	{
		if (glfwGetKey(window, moveKeys_[LEFT]) == GLFW_PRESS)
			moveVec.x--;

		if (glfwGetKey(window, moveKeys_[RIGHT]) == GLFW_PRESS)
			moveVec.x++;
	}

	if (vertical_)
	{
		if (glfwGetKey(window, moveKeys_[UP]) == GLFW_PRESS)
			moveVec.y++;

		if (glfwGetKey(window, moveKeys_[DOWN]) == GLFW_PRESS)
			moveVec.y--;
	}

	if (glm::length(moveVec) > 1.f)
		moveVec = glm::normalize(moveVec);
	rb_->AddForce(moveVec * speed_);
}

//ImGuiKey PlayerController::GetImGuiKey(unsigned int GLFWKey)
//{
//	unsigned int baseGLFWKey;
//	ImGuiKey baseImGuiKey;
//
//	if (GLFWKey >= GLFW_KEY_RIGHT)
//	{
//		switch (GLFWKey)
//		{
//		case GLFW_KEY_RIGHT:
//			return ImGuiKey_RightArrow;
//		case GLFW_KEY_LEFT:
//			return ImGuiKey_LeftArrow;
//		case GLFW_KEY_DOWN:
//			return ImGuiKey_DownArrow;
//		case GLFW_KEY_UP:
//			return ImGuiKey_UpArrow;
//		default:
//			return ImGuiKey_None;
//		}
//	}
//	else if (GLFWKey >= GLFW_KEY_A && GLFWKey <= GLFW_KEY_Z)
//	{
//		baseGLFWKey = GLFW_KEY_A;
//		baseImGuiKey = ImGuiKey_A;
//	}
//	else if (GLFWKey >= GLFW_KEY_0 && GLFWKey <= GLFW_KEY_9)
//	{
//		baseGLFWKey = GLFW_KEY_0;
//		baseImGuiKey = ImGuiKey_0;
//	}
//	else
//		return ImGuiKey_None;
//
//	return ImGuiKey(baseImGuiKey + (GLFWKey - baseGLFWKey));
//}

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

	//ImGui::Text("Move Keys");
	//static int selectedDir = -1;
	//ImGui::Combo("##moveKeys", &selectedDir, "LEFT\0RIGHT\0UP\0DOWN\0\0");
	//if (selectedDir != -1)
	//{
	//	ImGui::SameLine();
	//	ImGui::Text(ImGui::GetKeyName(GetImGuiKey(moveKeys_[selectedDir])));
	//}
	
	ImGui::Text("Speed");
	ImGui::InputFloat("##speed", &speed_, 1.f, 1.0f, "%.2f");
}

void PlayerController::SetMoveKeys(const unsigned int* keys)
{
	moveKeys_[LEFT] = keys[0];
	moveKeys_[RIGHT] = keys[1];
	moveKeys_[UP] = keys[2];
	moveKeys_[DOWN] = keys[3];
}

void PlayerController::SetMoveKey(Direction dir, unsigned int key)
{
	moveKeys_[dir] = key;
}

Component* PlayerController::CreateComponent(GameObject* owner)
{
	PlayerController* newComp = new PlayerController(owner);
	return static_cast<Component*>(newComp);
}
