#pragma once
#include "sigil.h"
using glm::vec3;
using glm::vec2;

class SigilBoard
{
private:
    glm::vec2 dimensions_;
    vector<Sigil*> sigils_;
public:
    SigilBoard(int width, int height);
    ~SigilBoard();
    void render(Shader* shader);
    void update(float dt, float t);
    vec2 getDimennsions(){return dimensions_;}
    Sigil* add(int x,int y);
};