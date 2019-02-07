#pragma once
#include <vector>
using std::vector;

#include <glm/glm.hpp>
#include "vertex.h"
#include "game_object.h"
#include "transform.h"
#include "core/boilerplate.h"
#include "rect.h"


class Sigil : GameObject
{
friend class SigilBoard;
public:
    static const int SIZE = 4;
    static constexpr float SCALE = 1.f;
    static constexpr bool blocks_S[SIZE][SIZE]={    {0,1,1,0},
                                                    {1,1,0,0},
                                                    {0,0,0,0},
                                                    {0,0,0,0}};
    static constexpr bool blocks_SR[SIZE][SIZE] = { {1,1,0,0},
                                                    {0,1,1,0},
                                                    {0,0,0,0},
                                                    {0,0,0,0}};
    static constexpr bool blocks_L[SIZE][SIZE] = {  {0,0,1,0},
                                                    {1,1,1,0},
                                                    {0,0,0,0},
                                                    {0,0,0,0}};
    static constexpr bool blocks_LR[SIZE][SIZE] =  {{1,1,1,0},
                                                    {0,0,1,0},
                                                    {0,0,0,0},
                                                    {0,0,0,0}};
    static constexpr bool blocks_Box[SIZE][SIZE] = {{1,1,0,0},
                                                    {1,1,0,0},
                                                    {0,0,0,0},
                                                    {0,0,0,0}};
    static constexpr bool blocks_Bar[SIZE][SIZE] = {{1,1,1,1},
                                                    {0,0,0,0},
                                                    {0,0,0,0},
                                                    {0,0,0,0}};                                                
    Sigil(const bool blocks[SIZE][SIZE], const glm::ivec2 pos);
    ~Sigil();
    void render(Shader* shader);
    void update(float dt, float t);
    inline glm::ivec2 position() const { return bounds_.position(); }
    void move(Boilerplate::Direction dir);
    inline const iRect* bounds() const { return &bounds_; }
private:
    void updateBounds();
    int width();
    int height();
    bool blocks_[Sigil::SIZE][Sigil::SIZE];
    vector<Vertex> vertices_;
    unsigned int vBO_, vAO_, eBO_;
    Transform transform_;
    iRect bounds_;
};
