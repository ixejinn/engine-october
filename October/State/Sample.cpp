#include "Sample.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "../Manager/GameObjectManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/FixedUpdatable/Transform.h"

namespace Manager
{
    extern GameObjectManager& objMgr;
}


void Sample::Init()
{
    /* VERTEX SHADER */
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /* FRAGMENT SHADER */
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // upper triangle
        1, 2, 3   // lower triangle
    };

    glGenVertexArrays(1, &VAO); // VAO stores the state related to vertex attribute settings
    glGenBuffers(1, &VBO);      // VBO stores the actual vertex data
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first,
    glBindVertexArray(VAO);

    // then bind and set vertex buffer(s),
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // Bind the VBO to a specific buffer target (GL_ARRAY_BUFFER)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Copy VBO to GL_ARRAY_BUFFER

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // and then configure vertex attributes(s)
    // (Connect the data of the VBO - which is bound to GL_ARRAY_BUFFER - to the vertex attribute array.
    // The VBO no longer needs to be bound to GL_ARRAY_BUFFER
    // The vertex data information of the VBO is stored in the VAO.)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Even if the binding of GL_ARRAY_BUFFER is released,
    // the necessary data for the vertex attributes is still accessible
    // (due to the glVertexAttribPointer call)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Releasing the binding of the VAO means that the state of this VAO can no longer be modified.
    // Even after unbinding, the vertex attributes and the data of the VBO remain intact.
    glBindVertexArray(0);
}

void Sample::Update()
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sample::Exit()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}