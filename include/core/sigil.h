#pragma once
#include <vector>
using std::vector;

#include <glm/glm.hpp>
#include "vertex.h"
#include "game_object.h"
#include "transform.h"
#include "core/boilerplate.h"
#include "rect.h"

typedef unsigned short block_def;

class Sigil : GameObject
{
friend class SigilBoard;
public:
    static const int SIZE = 4;
    static constexpr float SCALE = 1.f;
    static const block_def blocks_S =    (0b0110 <<SIZE * 3)
                                        +(0b1100 <<SIZE * 2)
                                        +(0b0000 <<SIZE * 1)
                                        +(0b0000 <<SIZE * 0);
    static const block_def blocks_SR =   (0b1100 <<SIZE * 3)
                                        +(0b0110 <<SIZE * 2)
                                        +(0b0000 <<SIZE * 1)
                                        +(0b0000 <<SIZE * 0);
    static const block_def blocks_L =    (0b0010 <<SIZE * 3)
                                        +(0b1110 <<SIZE * 2)
                                        +(0b0000 <<SIZE * 1)
                                        +(0b0000 <<SIZE * 0);
    static const block_def blocks_LR =   (0b1110 <<SIZE * 3)
                                        +(0b0010 <<SIZE * 2)
                                        +(0b0000 <<SIZE * 1)
                                        +(0b0000 <<SIZE * 0);
    static const block_def blocks_Box =  (0b1100 <<SIZE * 3)
                                        +(0b1100 <<SIZE * 2)
                                        +(0b0000 <<SIZE * 1)
                                        +(0b0000 <<SIZE * 0);
    static const block_def blocks_Bar =  (0b1111 <<SIZE * 3)
                                        +(0b0000 <<SIZE * 2)
                                        +(0b0000 <<SIZE * 1)
                                        +(0b0000 <<SIZE * 0);                                                
    Sigil(const block_def blocks, const glm::ivec2 pos);
    ~Sigil();
    void render(Shader* shader);
    void update(float dt, float t);
    inline glm::ivec2 position() const { return bounds_.position(); }
    void move(Boilerplate::Direction dir);
    inline const iRect* bounds() const { return &bounds_; }
    bool collidesWith(Sigil * s);
private:
    void updateBounds();
    int width();
    int height();
    const block_def blocks_;
    vector<Vertex> vertices_;
    unsigned int vBO_, vAO_, eBO_;
    Transform transform_;
    iRect bounds_;
    void dump();
    bool blockAt(int row, int col);
};
