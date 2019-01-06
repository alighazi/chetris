#include "core/GameWorld.h"
void GameWorld::addObject(GameObject* obj)
{
    this->_objects.push_back(obj);
}

void GameWorld::update(float dt, float elapsed)
{
    for(int i=0;i<_objects.size(); i++){
        _objects[i]->update(dt,elapsed);
    }
}

void GameWorld::render(Shader* shader)
{
    for(int i=0;i<_objects.size(); i++){
        _objects[i]->render(shader);
    }
}