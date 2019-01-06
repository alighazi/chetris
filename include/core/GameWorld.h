#pragma once

#include <vector>
#include "game_object.h"
#include "Camera.h"

class GameWorld{
    private:
    std::vector<GameObject*> _objects;
    Camera _camera;

    public:
    static const int WINDOW_WIDTH = 1024;
    static const int WINDOW_HEIGHT = 768;
    GameWorld():_camera(WINDOW_WIDTH, WINDOW_HEIGHT){}
    void addObject(GameObject* obj);
    void update(float dt, float elapsed);
    void render(Shader* shader);
    inline Camera* getCam() {return &_camera;}

    //singletone
    static GameWorld& instance() 
    {
        static GameWorld instance;
        return instance;
    }
    GameWorld(GameWorld const&)      = delete;
    void operator=(GameWorld const&)  = delete;
};
