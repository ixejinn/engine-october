#pragma once
#include <glm/vec2.hpp>

#include "Component.h"

class Transform;

class Rigidbody : public Component
{
private:
	float mass_{ 1.f };
	float drag_{ 0.f };
	float restitution_{ 0.5f };

	glm::vec2 force_{ 0.f, 0.f };
	glm::vec2 velocity_{ 0.f, 0.f };

	Transform* trans_{ nullptr };

	Rigidbody(GameObject* owner);

public:
	~Rigidbody();

	void Update();

	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void ShowDetails() override;
	
	float GetMass() const { return mass_; }
	float GetRestitution() const { return restitution_; }

	void SetVelocity(const glm::vec2& velocity);

	void AddForce(const glm::vec2& f);
	void AddForce(const float& fx, const float& fy);

	static Component* CreateComponent(GameObject* owner);
};