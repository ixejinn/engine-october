#pragma once
#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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

	GLfloat alpha_ = 1.0f;
	glm::vec3 colors_[4];

	GLuint VBO[3], VAO, EBO;
	GLuint texture;

	Shader shader;
	const char* vertexShaderSource = nullptr;
	const char* fragmentShaderSource = nullptr;

	Transform* trans_ = nullptr;

	void LateUpdate() override;

	void SetMesh();

	Sprite(GameObject* owner);

public:
	~Sprite();

	// @param idx is vertex of rectangle
	// (0 up right, 1 down right, 2 down left, 3 up left)
	void SetColor(const unsigned int& idx, const glm::vec3& color);
	void SetTexture(const char* name);

	static Component* CreateComponent(GameObject* owner);
};