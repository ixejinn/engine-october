#pragma once
#include "Collider.h"

class CircleCollider : public Collider
{
private:
	float radius_{};

	CircleCollider(GameObject* owner);

	void FixedUpdate() override;

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void ShowDetails() override;

	void SetScaleWithTransform(bool b) override;

	static Component* CreateComponent(GameObject* owner);

	friend class CollisionManager;
};