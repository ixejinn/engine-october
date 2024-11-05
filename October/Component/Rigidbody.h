#pragma once
#include <glm/vec2.hpp>

#include "Component.h"

class Transform;

class Rigidbody : public Component
{
private:
	float mass_{ 1.f };
	float drag_{ 0.f };

	glm::vec2 force_{ 0.f, 0.f };
	glm::vec2 velocity_{ 0.f, 0.f };

	Transform* trans_{ nullptr };

	Rigidbody(GameObject* owner);

	bool CheckEpsilon(float val, float epsilon = 0.00001f);

public:
	~Rigidbody();

	void Update();

	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void ShowDetails() override;

	void AddForce(const glm::vec2& f);
	void AddForce(const float& fx, const float& fy);

	static Component* CreateComponent(GameObject* owner);
};