#pragma once
#include <string>
#include "State.h"

class Sample : public State
{
private:
	unsigned int vertexShader = 0;

    const char* vertexShaderSource;
    const char* fragmentShaderSource;

    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    unsigned int texture;

public:
	void Init() override;
	void Update() override;
	void Exit() override;
};