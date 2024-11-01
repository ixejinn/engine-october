#pragma once
#include <GLFW/glfw3.h>

#include "../Component.h"
#include "Updatable.h"
#include "../../Utils/Utils.h"

class Rigidbody;

class PlayerController : public Component, public Updatable
{
private:
	bool vertical_{ true };
	bool horizontal_{ true };

	unsigned int moveKeys_[4]{ GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S };

	float speed_{ 1.f };

	Rigidbody* rb_{ nullptr };

	PlayerController(GameObject* owner);

	void Update() override;

	// @brief Convert GLFW_KEY to ImGuiKey only for arrow, number and alphabet keys
	//static ImGuiKey GetImGuiKey(unsigned int GLFWkey);

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void ShowDetails() override;

	// @brief Key order is LEFT, RIGHT, UP, DOWN
	void SetMoveKeys(const unsigned int* keys);
	void SetMoveKey(Direction dir, unsigned int key);

	static Component* CreateComponent(GameObject* owner);
};