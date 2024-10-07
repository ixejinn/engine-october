#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../Component.h"
#include "LateUpdatable.h"

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

	glm::vec3 color_ = { 0.f, 0.f, 0.f };
	GLfloat alpha_ = 1.0f;

	unsigned int vertexShader = 0;

	const char* vertexShaderSource;
	const char* fragmentShaderSource;

	unsigned int shaderProgram;
	unsigned int VBO, VAO, EBO;
	unsigned int texture;

	Transform* trans_ = nullptr;

	void LateUpdate() override;

	Sprite(GameObject* owner);

public:
	~Sprite();

	static Component* CreateComponent(GameObject* owner);
};