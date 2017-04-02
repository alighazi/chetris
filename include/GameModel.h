#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include <glm/glm.hpp>
#include "GameObject.h"
#include "shader.h"
#include "Model.h"

class GameModel: GameObject
{
    private:
        Model _model;
        Shader* _shader;
        glm::vec3 _position;
        glm::vec3 _scale;
        glm::mat4 _transform;
    public:
        GameModel(const char* modelFile, Shader* shader);
        void update(float dt, float elapsed);
        void render();
        ~GameModel();
};

#endif