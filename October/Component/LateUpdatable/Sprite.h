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
	Shader shader;
	AnchorPoint anchor_ = CENTER;
	glm::vec2 localPosition_ = { 0.f, 0.f };

	glm::vec3 color_ = { 0.f, 0.f, 0.f };
	GLfloat alpha_ = 1.0f;

	GLuint vertexShader = 0;

	const char* vertexShaderSource = nullptr;
	const char* fragmentShaderSource = nullptr;

	GLuint VBO, VAO, EBO;
	GLuint texture;

	Transform* trans_ = nullptr;

	void LateUpdate() override;

	void SetMesh();

	Sprite(GameObject* owner);

public:
	~Sprite();

	static Component* CreateComponent(GameObject* owner);
};