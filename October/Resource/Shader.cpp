#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::~Shader()
{
    Unload();
}

void Shader::Load(const std::string& fsName)
{
    std::string vsName = "Assets/Shaders/shader.vs";
    fsName_ = fsName;

    /* READ SHADER SOURCE FILE */
    std::string vertexSrcStr;
    std::string fragmentSrcStr;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertexFile.open(vsName);
        fragmentFile.open(fsName);

        std::stringstream vertexStream, fragmentStream;
        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexSrcStr = vertexStream.str();
        fragmentSrcStr = fragmentStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "[ERROR] Shader::Load() Invalid filename " << fsName << std::endl;
    }

    const char* vertexSrc = vertexSrcStr.c_str();
    const char* fragmentSrc = fragmentSrcStr.c_str();

    /* COMPILE SHADER */
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);
    CheckCompileErros(vertexShader, VERTEX);

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);
    CheckCompileErros(fragmentShader, FRAGMENT);

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckCompileErros(shaderProgram, PROGRAM);
    data_ = static_cast<void*>(&shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Unload()
{
    glDeleteProgram(GetData());
}

void Shader::Use()
{
    if (data_ == nullptr)
        Load(BasicFragmentShaderName);   // basic fragment shader

    glUseProgram(GetData());
}

unsigned int Shader::GetData()
{
    return *(static_cast<unsigned int*>(data_));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(
        glGetUniformLocation(GetData(), name.c_str()),
        1,
        GL_FALSE,
        glm::value_ptr(mat)
    );
}

void Shader::SetUniform1f(const std::string& name, const float& f)
{
    glUniform1f(
        glGetUniformLocation(GetData(), name.c_str()),
        f
    );
}

void Shader::CheckCompileErros(unsigned int shader, Type type)
{
    int success;
    char infoLog[512];
    
    switch (type)
    {
    case VERTEX:
    case FRAGMENT:
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "[ERROR] Shader::CheckCompileErrors() Shader compile error\n" <<
                "type: " << (type == VERTEX ? "VERTEX" : "FRAGMENT") << "\n" <<
                infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
        break;

    case PROGRAM:
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "[ERROR] Shader::CheckCompileErrors() Program linking error\n" <<
                infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}