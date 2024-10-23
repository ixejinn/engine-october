#pragma once
#include <GLFW/glfw3.h>

#include "../Component.h"
#include "Updatable.h"

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

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void ShowDetails() override;

	static Component* CreateComponent(GameObject* owner);
};