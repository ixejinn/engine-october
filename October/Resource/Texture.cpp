#include "Texture.h"

#include "../Utils/stb/stb_image.h"

Texture::~Texture()
{
	Unload();
}

void Texture::Load(const std::string& filename)
{
	textureName_ = filename;

	GLuint* texture = new GLuint();
	glCreateTextures(GL_TEXTURE_2D, 1, texture);

	glTextureParameteri(*texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(*texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureParameteri(*texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(*texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(textureName_.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data)
    {
        glTextureStorage2D(*texture, 1, GL_RGBA8, width, height);
        glTextureSubImage2D(*texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glGenerateTextureMipmap(*texture);

        stbi_image_free(data);
    }
    else
    {
        delete texture;
        texture = nullptr;
    }

    data_ = texture;
}

void Texture::Unload()
{
    if (data_ != nullptr)
        glDeleteTextures(1, static_cast<GLuint*>(data_));

    data_ = nullptr;
}

GLuint Texture::GetData()
{
    return *(static_cast<GLuint*>(data_));
}

Texture* Texture::BasicTexture()
{
    static Texture basicTexture;
    if (basicTexture.data_ == nullptr)
        CreateBasicTexture(&basicTexture);

    return &basicTexture;
}

void Texture::CreateBasicTexture(Texture* basicTexture)
{
    GLuint* texture = new GLuint();
    glCreateTextures(GL_TEXTURE_2D, 1, texture);

    GLubyte white[4] = { 255, 255, 255, 255 };
    glTextureStorage2D(*texture, 1, GL_RGBA8, 1, 1);
    glTextureSubImage2D(*texture, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, white);

    basicTexture->data_ = texture;
}