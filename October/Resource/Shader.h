#pragma once
#include <string>
#include <glm/mat4x4.hpp>

class Shader
{
public:
	enum Type
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

private:
	unsigned int shaderProgram;

	void CheckCompileErros(unsigned int shader, Type type);

public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void Use();

	void SetTextureAvailable(unsigned int texture);
	void SetUniformMat4(const std::string& name, const glm::mat4& mat);
};