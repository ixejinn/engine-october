#pragma once
#include "State.h"

class Sample : public State
{
private:
	unsigned int vertexShader = 0;

    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"
        "out vec3 ourColor;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "	ourColor = aColor;\n"
        "	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D texture1;\n"
        "void main()\n"
        "{\n"
        "   FragColor = texture(texture1, TexCoord);\n"
        "}\n\0";

    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    unsigned int texture;

public:
	void Init() override;
	void Update() override;
	void Exit() override;
};