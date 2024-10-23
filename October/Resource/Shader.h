#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include "Resource.h"

class Shader : public Resource
{
private:
	enum Type
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	std::string fsName_{};

	Shader() = default;

	void CheckCompileErros(GLuint shader, Type type);

public:
	~Shader();

	// The vertex shaders are all the same
	void Load(const std::string& fsName) override;
	void Unload() override;

	void Use();

	GLuint GetData();

	const std::string& GetFragmentShaderName() { return fsName_; }

	void SetUniformMat4(const std::string& name, const glm::mat4& mat);
	void SetUniform1f(const std::string& name, const GLfloat& f);

	static constexpr const char* BasicFragmentShaderName_ = "Assets/Shaders/shader.fs";

	friend class ResourceManager;
};