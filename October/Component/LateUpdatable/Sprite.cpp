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
#include "../../Resource/Texture.h"

namespace Manager
{
    extern ResourceManager& rscMgr;
}

Sprite::Sprite(GameObject* owner) : Component(owner)
{
    updateInEditmode_ = true;

    trans_ = static_cast<Transform*>(owner_->GetComponent(typeid(Transform)));

    shader_ = Manager::rscMgr.Load<Shader>(Shader::BasicFragmentShaderName_);

    SetMesh();
    texture_ = Texture::BasicTexture();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Sprite::~Sprite()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(3, vbo_);
    glDeleteBuffers(1, &ebo_);
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
        std::string textureName = it.value();
        if (!textureName.empty())
            SetTexture(textureName);
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
    compData["textureName"] = texture_->GetName();

    data["compData"] = compData;
    return data;
}

const std::string& Sprite::GetTextureName()
{
    return texture_->GetName();
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
    Manager::rscMgr.Unload(shader_->GetFragmentShaderName());
    shader_ = Manager::rscMgr.Load<Shader>(name);
}

void Sprite::SetTexture(const std::string& name)
{
    if (texture_ != nullptr && texture_ != Texture::BasicTexture())
        Manager::rscMgr.Unload(texture_->GetName());
    texture_ = Manager::rscMgr.Load<Texture>(name);
}

Component* Sprite::CreateComponent(GameObject* owner)
{
    Sprite* newComp = new Sprite(owner);
    return static_cast<Sprite*>(newComp);
}

void Sprite::LateUpdate()
{
    // Change color
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors_), colors_);

    // Activate shader
    shader_->Use();

    glm::mat4 transformMatrix = glm::mat4(1.f);
    transformMatrix = glm::mat4(trans_->GetMatrix());
    shader_->SetUniformMat4("transform", transformMatrix);

    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view, glm::vec3(0.f, 0.f, -10.f));
    shader_->SetUniformMat4("view", view);

    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(glm::radians(45.f), 1500 / 1000.f, 0.1f, 100.f);
    shader_->SetUniformMat4("projection", projection);

    shader_->SetUniform1f("alpha", alpha_);

    glBindTexture(GL_TEXTURE_2D, texture_->GetData());
    glBindVertexArray(vao_);
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

    glGenVertexArrays(1, &vao_); // VAO stores the state related to vertex attribute settings
    glGenBuffers(3, vbo_);       // VBO stores the actual vertex data (positions, colors, texture coords)
    glGenBuffers(1, &ebo_);      // EBO stores the indices used for element drawing (index array)

    // Bind the Vertex Array Object first,
    glBindVertexArray(vao_);

    // Bind and set Vertex Buffer Object (GL_ARRAY_BUFFER)
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);    // Allocate memory and insert data(or NULL)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors_), colors_, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    // Bind the Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}
