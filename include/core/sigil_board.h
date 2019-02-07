#pragma once
#include "sigil.h"
#include "input_handler.h"
using glm::vec3;
using glm::vec2;

class SigilBoard: GameObject, InputHandler
{
private:
    glm::vec2 dimensions_;
    Sigil *sigil_;
public:
    SigilBoard(int width, int height);
    ~SigilBoard();
    void render(Shader* shader);
    void update(float dt, float t);
    void onKeyRelease(int key);
    vec2 getDimennsions(){return dimensions_;}
};