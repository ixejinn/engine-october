#pragma once
#include <string>
#include <glm/mat4x4.hpp>
#include "Resource.h"

class Shader : public Resource
{
public:
	enum Type
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

private:
	//unsigned int shaderProgram;

	Shader() = default;

	void CheckCompileErros(unsigned int shader, Type type);

public:
	~Shader();

	void Load(const std::string& filename) override;
	void Unload() override;

	void Use();

	unsigned int GetData();

	void SetUniformMat4(const std::string& name, const glm::mat4& mat);
	void SetUniform1f(const std::string& name, const float& f);
};