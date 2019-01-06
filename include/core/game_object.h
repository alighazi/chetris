#pragma once
#include "util/Shader.h"

class GameObject
{
    public:
        virtual void update(float dt, float elapsed) = 0;
        virtual void render(Shader* shader) = 0;
};