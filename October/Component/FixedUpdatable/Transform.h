#pragma once
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>
#include "../Component.h"
#include "FixedUpdatable.h"

class Transform : public Component, public FixedUpdatable
{
private:
	glm::vec2 position_ = { 0.f, 0.f };
	float rotation_ = 0.f;
	glm::vec2 scale_ = { 1.f, 1.f };
	glm::vec2 localScale_ = { 1.f, 1.f };

	glm::mat3 transformMatrix_;

	void UpdateMatrix();
	void FixedUpdate() override;

	Transform(GameObject* owner);

public:
	const glm::vec2& GetPosition() const { return position_; }
	const float& GetRotation() const { return rotation_; }
	const glm::vec2& GetScale() const { return scale_; }
	const glm::vec2& GetLocalScale() const { return localScale_; }
	const glm::mat3& GetMatrix() const { return transformMatrix_; }

	void SetPosition(const glm::vec2& pos);
	void SetPosition(const float& x, const float& y);
	void SetScale(const glm::vec2& scl);
	void SetLocalScale(const glm::vec2& scl);
	void SetRotation(const glm::vec2& rot);
	void SetRotation(const float& rot);
	void SetFlip();

	static Component* CreateComponent(GameObject* owner);
};