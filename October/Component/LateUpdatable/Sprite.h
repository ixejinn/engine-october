#pragma once
#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include "../Component.h"
#include "LateUpdatable.h"
#include "../../Resource/Shader.h"

class Transform;

class Sprite : public Component, public LateUpdatable
{
public:
	enum AnchorPoint
	{
		CENTER,
		LEFT_CENTER,
		LEFT_UP
	};

private:
	AnchorPoint anchor_ = CENTER;
	glm::vec2 localPosition_ = { 0.f, 0.f };

	glm::vec3 colors_[4] = {
		{ 1.f, 1.f, 1.f },
		{ 1.f, 1.f, 1.f },
		{ 1.f, 1.f, 1.f },
		{ 1.f, 1.f, 1.f }
	};
	GLfloat alpha_ = 1.0f;

	GLuint VBO[3], VAO, EBO;
	Shader shader;
	GLuint texture;
	std::string textureName_;

	Transform* trans_ = nullptr;

	Sprite(GameObject* owner);

	void LateUpdate() override;

	void SetMesh();

public:
	~Sprite();

	void LoadFromJson(const json& data) override;
	json SaveToJson() override;

	const std::string& GetTextureName() { return textureName_; }

	float* GetLocalPosition() { return &localPosition_[0]; }
	float* GetColor(const unsigned int& idx) { return &colors_[idx][0]; }
	float* GetAlpha() { return &alpha_; }

	// @param idx is vertex of rectangle
	// (0 up right, 1 down right, 2 down left, 3 up left)
	void SetColor(const unsigned int& idx, const glm::vec3& color);
	void SetAlpha(const float& alpha);
	void SetTexture(const std::string& name);

	static Component* CreateComponent(GameObject* owner);
};