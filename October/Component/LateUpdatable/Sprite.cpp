#include "Sprite.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../FixedUpdatable/Transform.h"
#include "../../GameObject/GameObject.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utils/stb/stb_image.h"
#include "../../Resource/Shader.h"

namespace Manager
{
    extern ResourceManager& rscMgr;
}

Sprite::Sprite(GameObject* owner) : Component(owner)
{
    trans_ = static_cast<Transform*>(owner_->GetComponent(typeid(Transform)));

    shader = Manager::rscMgr.Load<Shader>(Shader::BasicFragmentShaderName);

    SetMesh();
    SetTexture("");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Sprite::~Sprite()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(3, VBO);
    glDeleteBuffers(1, &EBO);
}

void Sprite::LoadFromJson(const json& data)
{
    auto compData = data.find("compData");
    if (compData != data.end())
    {
        auto it = compData->find("anchor");
        anchor_ = it.value();

        it = compData->find("localPosition");
        localPosition_.x = it->begin().value();
        localPosition_.y = (it->begin() + 1).value();

        it = compData->find("colors");
        auto v = it->begin();
        for (int i = 0; i < 4; i++)
        {
            colors_[i].r = v.value();
            colors_[i].g = (v + 1).value();
            colors_[i].b = (v + 2).value();

            v = v + 3;
        }

        it = compData->find("alpha");
        alpha_ = it.value();

        it = compData->find("textureName");
        textureName_ = it.value();
        SetTexture(textureName_);
    }
}

json Sprite::SaveToJson()
{
    json data, compData;
    data["type"] = typeid(Sprite).name();

    compData["anchor"] = anchor_;
    compData["localPosition"] = { localPosition_.x, localPosition_.y };
    compData["colors"] = {
        colors_[0].r, colors_[0].g, colors_[0].b,
        colors_[1].r, colors_[1].g, colors_[1].b,
        colors_[2].r, colors_[2].g, colors_[2].b,
        colors_[3].r, colors_[3].g, colors_[3].b
    };
    compData["alpha"] = alpha_;
    compData["textureName"] = textureName_;

    data["compData"] = compData;
    return data;
}

void Sprite::SetColor(const unsigned int& idx, const glm::vec3& color)
{
    colors_[idx] = color;
}

void Sprite::SetAlpha(const float& alpha)
{
    alpha_ = alpha;
}

void Sprite::SetFragmentShader(const std::string& name)
{
    Manager::rscMgr.Unload(shader->GetFragmentShaderName());
    shader = Manager::rscMgr.Load<Shader>(name);
}

void Sprite::SetTexture(const std::string& name)
{
    textureName_ = name;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(textureName_.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data)
    {
        glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glGenerateTextureMipmap(texture);

        stbi_image_free(data);
    }
    else
    {
        GLubyte white[4] = { 255, 255, 255, 255 };
        glTextureStorage2D(texture, 1, GL_RGBA8, 1, 1);
        glTextureSubImage2D(texture, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, white);
    }
}

Component* Sprite::CreateComponent(GameObject* owner)
{
    Sprite* newComp = new Sprite(owner);
    return static_cast<Sprite*>(newComp);
}

void Sprite::LateUpdate()
{
    // Change color
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors_), colors_);

    // Activate shader
    shader->Use();

    glm::mat4 transformMatrix = glm::mat4(1.f);
    transformMatrix = glm::mat4(trans_->GetMatrix());
    shader->SetUniformMat4("transform", transformMatrix);

    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view, glm::vec3(0.f, 0.f, -10.f));
    shader->SetUniformMat4("view", view);

    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(glm::radians(45.f), 1500 / 1000.f, 0.1f, 100.f);
    shader->SetUniformMat4("projection", projection);

    shader->SetUniform1f("alpha", alpha_);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite::SetMesh()
{
    glm::vec3 positions[] = {
        {  0.5f,  0.5f, 0.f },
        {  0.5f, -0.5f, 0.f },
        { -0.5f, -0.5f, 0.f },
        { -0.5f,  0.5f, 0.f }
    };
    glm::vec2 textureCoords[] = {
        { 1.f, 0.f },
        { 1.f, 1.f },
        { 0.f, 1.f },
        { 0.f, 0.f }
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO); // VAO stores the state related to vertex attribute settings
    glGenBuffers(3, VBO);       // VBO stores the actual vertex data (positions, colors, texture coords)
    glGenBuffers(1, &EBO);      // EBO stores the indices used for element drawing (index array)

    // Bind the Vertex Array Object first,
    glBindVertexArray(VAO);

    // Bind and set Vertex Buffer Object (GL_ARRAY_BUFFER)
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);    // Allocate memory and insert data(or NULL)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors_), colors_, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    // Bind the Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}
