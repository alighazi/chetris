// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// #include "GameModel.h"
// #include "GameWorld.h"
// using glm::vec3;

// GameModel::GameModel(const char* modelFile, Shader* shader):_shader(shader),_model(modelFile)
// {
// }

// void GameModel::update(float dt, float elapsed){
// }


// void GameModel::render(){
//     _shader->Use();
//     glm::mat4 vp = GameWorld::instance().getCam()->getViewProjection();
// 	glUniformMatrix4fv(glGetUniformLocation(_shader->Program, "viewProj"), 1, GL_FALSE, glm::value_ptr(vp));
//     _transform = glm::translate(glm::mat4(1.0f), _position);
//     _transform = glm::scale(_transform, _scale);
//     glUniformMatrix4fv(glGetUniformLocation(_shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(_transform));
//     _model.Draw(_shader);
// }

// GameModel::~GameModel(){}