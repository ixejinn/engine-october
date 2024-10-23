#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
private:
	glm::vec2 scale_{};

	BoxCollider(GameObject* owner);

	void FixedUpdate() override;

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void ShowDetails() override;

	static Component* CreateComponent(GameObject* owner);
};