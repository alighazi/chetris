#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameModel.h"
#include "GameWorld.h"
using glm::vec3;

GameModel::GameModel(const char* modelFile, Shader* shader):_shader(shader),_model(modelFile)
{
    _position = vec3(1.0f, 2.75f, -2.5f);
    _scale = vec3(0.1f,0.1f,0.1f);
}

void GameModel::update(float dt, float elapsed){
    _position.x = cos(elapsed)*4.0f;
    _position.z = sin(elapsed)*4.0f - 5.0f;
}

void GameModel::render(){
    _shader->Use();
    glm::mat4 vp = GameWorld::instance().getCam()->getViewProjection();
	glUniformMatrix4fv(glGetUniformLocation(_shader->Program, "viewProj"), 1, GL_FALSE, glm::value_ptr(vp));
    _transform = glm::translate(glm::mat4(1.0f), _position);
    _transform = glm::scale(_transform, _scale);
    glUniformMatrix4fv(glGetUniformLocation(_shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(_transform));
    _model.Draw(_shader);
}

GameModel::~GameModel(){}