#include "core/sigil_board.h"

SigilBoard::SigilBoard(int width, int height)
{
    dimensions_=glm::vec2(width,height);
    sigils_.push_back(new Sigil(Sigil::blocks_S, vec3(0.f, 0.f, 1.f)));
    sigils_.push_back(new Sigil(Sigil::blocks_Box, vec3(0.f, 0.f, 1.f)));
    sigils_.push_back(new Sigil(Sigil::blocks_Bar, vec3(0.f, 0.f, 1.f)));
    sigils_.push_back(new Sigil(Sigil::blocks_L, vec3(0.f, 0.f, 1.f)));
    sigils_.push_back(new Sigil(Sigil::blocks_LR, vec3(0.f, 0.f, 1.f)));
    sigils_.push_back(new Sigil(Sigil::blocks_SR, vec3(0.f, 0.f, 1.f)));
}

SigilBoard::~SigilBoard()
{
    for(int i=0; i<sigils_.size(); i++){
        delete sigils_[i];
        sigils_.clear();
    }
}

void SigilBoard::render(Shader* shader){
    for(int i=0; i<sigils_.size(); i++){
        sigils_[i]->render(shader);
    }
}

void SigilBoard::update(float dt, float t){
    for(int i=0; i<sigils_.size(); i++){
        sigils_[i]->update(dt, t);
    }
}