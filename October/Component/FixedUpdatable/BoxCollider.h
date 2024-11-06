#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
private:
	glm::vec2 scale_{ 1.f, 1.f };	// Relative with Transform's scale

	BoxCollider(GameObject* owner);

	void FixedUpdate() override;

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void ShowDetails() override;

	void SetScale(bool withTransform, glm::vec2 scale = { 1.f, 1.f });

	void SetColliderType(ColliderType type);

	static Component* CreateComponent(GameObject* owner);

	friend class CollisionManager;
};