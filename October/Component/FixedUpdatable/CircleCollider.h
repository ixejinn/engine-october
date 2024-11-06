#pragma once
#include "Collider.h"

class CircleCollider : public Collider
{
private:
	float radius_{ 0.5f };	// Relative with Transform's scale

	CircleCollider(GameObject* owner);

	void FixedUpdate() override;

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void ShowDetails() override;

	void SetRadius(bool withTransform, float radius = 0.5f);

	static Component* CreateComponent(GameObject* owner);

	friend class CollisionManager;
};