#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
private:
	glm::vec2 normals_[4]{};
	glm::vec2 scale_{};

	BoxCollider(GameObject* owner);

	void FixedUpdate() override;

public:
	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	void ShowDetails() override;

	void SetScaleWithTransform(bool b) override;

	const glm::vec2& GetVertex(unsigned int idx) const { return vertices_[idx]; }
	const glm::vec2& GetNormal(unsigned int idx) const { return normals_[idx]; }

	static Component* CreateComponent(GameObject* owner);

	friend class CollisionManager;
};