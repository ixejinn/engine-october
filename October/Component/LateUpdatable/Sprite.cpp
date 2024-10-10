#include "Sprite.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../FixedUpdatable/Transform.h"
#include "../../GameObject/GameObject.h"
#include "../../Utils/stb/stb_image.h"
#include "../../Resource/Shader.h"

Sprite::Sprite(GameObject* owner) : Component(owner), shader("Assets/Shaders/shader.vs", "Assets/Shaders/shader.fs")
{
    trans_ = static_cast<Transform*>(owner_->GetComponent(typeid(Transform)));

    SetMesh();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Assets/IMG_4166.JPEG", &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Sprite::~Sprite()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(3, VBO);
    glDeleteBuffers(1, &EBO);
}

Component* Sprite::CreateComponent(GameObject* owner)
{
    Sprite* newComp = new Sprite(owner);
    return static_cast<Sprite*>(newComp);
}

void Sprite::LateUpdate()
{
    shader.Use();
    shader.SetTextureAvailable(texture);

    glm::mat4 transformMatrix = glm::mat4(1.f);
    transformMatrix = glm::mat4(trans_->GetMatrix());
    shader.SetUniformMat4("transform", transformMatrix);

    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view, glm::vec3(0.f, 0.f, -10.f));
    shader.SetUniformMat4("view", view);

    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(glm::radians(45.f), 1500 / 1000.f, 0.1f, 100.f);
    shader.SetUniformMat4("projection", projection);

    //glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite::SetMesh()
{
    float positions[] = {
        0.5f,  0.5f, 0.f,
        0.5f, -0.5f, 0.f,
       -0.5f, -0.5f, 0.f,
       -0.5f,  0.5f, 0.f
    };
    float colors[] = {
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f
    };
    float textureCoords[] = {
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 0.f
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO); // VAO stores the state related to vertex attribute settings
    glGenBuffers(3, VBO);       // VBO stores the actual vertex data (positions, colors, texture coords)
    glGenBuffers(1, &EBO);      // EBO stores the indices used for element drawing (index array)

    // bind the Vertex Array Object first,
    glBindVertexArray(VAO);

    // then bind and set vertex buffers,
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);     // Bind the VBO to a specific buffer target (GL_ARRAY_BUFFER)
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
