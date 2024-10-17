#pragma once
#include <glad/glad.h>
#include "Resource.h"

class Texture : public Resource
{
private:
	std::string textureName_ = "";

	Texture() = default;

public:
	~Texture();

	void Load(const std::string& filename) override;
	void Unload() override;

	GLuint GetData();

	const std::string& GetName() { return textureName_; }

	static constexpr const char* BasicTexturename = "virtual.png";

	friend class ResourceManager;
};