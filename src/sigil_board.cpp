#include "core/sigil_board.h"
#include "core/boilerplate.h"

SigilBoard::SigilBoard(int width, int height)
{
    dimensions_= glm::vec2(width,height);
    Sigil* s= new Sigil(Sigil::blocks_S, vec3(0.f,5.f, 1.f), vec2(0.f,-1.f));
    s->position.y= height - s->height();
    sigils_.push_back(s);
    Boilerplate::instance().addInputConsumer(this);    
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

Sigil* SigilBoard::add(int x,int y){
    Sigil* s= new Sigil(Sigil::blocks_S, vec3(x, y, 1.f));
    sigils_.push_back(s);
    return s;
}

void SigilBoard::onKeyRelease(int key){
    Boilerplate::Direction dir = Boilerplate::NONE;
    if(key == GLFW_KEY_LEFT) dir = Boilerplate::LEFT;
    else if(key == GLFW_KEY_RIGHT) dir = Boilerplate::RIGHT;
    else return;
    
    for(int i=0; i<sigils_.size(); i++){
        sigils_[i]->move(dir);
    }
}