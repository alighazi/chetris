#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/util/Shader.h"

class Boilerplate
{
private:
public:
    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    GLFWwindow* window;
    Shader* shader;
    unsigned int texture1, texture2;
    void update(float deltaTime);
    Boilerplate(/* args */);
    ~Boilerplate();
};