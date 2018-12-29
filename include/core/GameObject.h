#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject
{
    public:
        virtual void update(float dt, float elapsed) = 0;
        virtual void render() = 0;
};
#endif