#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "core/util/Shader.h"
#include "core/input_consumer.h"

class Boilerplate
{
private:
    //private constructor, we don't want instantiation to be possible
    std::vector<InputConsumer*> input_consumers_;
    Boilerplate();
    //libgdx callbacks
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

public:

    enum Direction{
        NONE = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 4,
        LEFT = 8
    };
    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    GLFWwindow* window;
    Shader* shader;
    unsigned int texture1, texture2;
    void update(float deltaTime);
    static Boilerplate& instance() {
        static Boilerplate bp;
        return bp;
    }
    ~Boilerplate();
    Boilerplate(Boilerplate const&)     = delete;
    void operator=(Boilerplate const&)  = delete;
    void addInputConsumer(InputConsumer* ic);
};