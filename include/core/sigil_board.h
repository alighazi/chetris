#pragma once
#include "sigil.h"
#include "input_consumer.h"
using glm::vec3;
using glm::vec2;

class SigilBoard: GameObject, InputConsumer
{
private:
    glm::vec2 dimensions_;
    vector<Sigil*> sigils_;
public:
    SigilBoard(int width, int height);
    ~SigilBoard();
    void render(Shader* shader);
    void update(float dt, float t);
    void onKeyRelease(int key);
    vec2 getDimennsions(){return dimensions_;}
    Sigil* add(int x,int y);
};