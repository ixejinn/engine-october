#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../Component.h"
#include "LateUpdatable.h"

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


};