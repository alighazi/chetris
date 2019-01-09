#pragma once
#include "sigil.h"
using glm::vec3;

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
    glm::vec2 getDimennsions(){return dimensions_;}
};