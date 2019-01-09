#pragma once
#include <vector>
using std::vector;

#include <glm/glm.hpp>
#include "vertex.h"
#include "game_object.h"
#include "transform.h"


class Sigil : GameObject
{
friend class SigilBoard;
public:
    static const int SIZE = 4;
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
    glm::ivec2 position;
private:
    bool blocks_[Sigil::SIZE][Sigil::SIZE];
    vector<Vertex> vertices_;
    unsigned int vBO_, vAO_, eBO_;
    Transform transform_;
};
