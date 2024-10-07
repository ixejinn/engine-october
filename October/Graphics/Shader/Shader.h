#pragma once

class Shader
{
private:
	const char* vertexShaderSource;
	const char* fragmentShaderSource;

	unsigned int shaderProgram;

public:
	Shader();
};