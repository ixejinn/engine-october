#pragma once
#include <glad/glad.h>
#include "Resource.h"

class Texture : public Resource
{
private:
	std::string textureName_{};

	Texture() = default;

	static void CreateBasicTexture(Texture* basicTexture);

public:
	~Texture();

	void Load(const std::string& filename) override;
	void Unload() override;

	GLuint GetData();

	const std::string& GetName() { return textureName_; }

	static Texture* BasicTexture();

	friend class ResourceManager;
};