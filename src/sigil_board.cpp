#include "core/sigil_board.h"
#include "core/boilerplate.h"

SigilBoard::SigilBoard(int width, int height)
{
    dimensions_= glm::vec2(width,height);
    sigil_ =  new Sigil(Sigil::blocks_S, vec3(0.f,5.f, 1.f));
    Boilerplate::instance().addInputHandler(this);    
}

SigilBoard::~SigilBoard()
{
    delete sigil_;
    sigil_ = nullptr;
}

void SigilBoard::render(Shader* shader){
    sigil_->render(shader);
}

void SigilBoard::update(float dt, float t){    
    static float last_frame=0.f;
    last_frame += dt;
    if(last_frame < 1.f)
        return;
    if(sigil_->bounds()->bottom() >0 )
        sigil_->update(last_frame, t);
    last_frame = 0.f;
}

void SigilBoard::onKeyRelease(int key){
    Boilerplate::Direction dir = Boilerplate::NONE;
    if(key == GLFW_KEY_LEFT) dir = Boilerplate::LEFT;
    else if(key == GLFW_KEY_RIGHT) dir = Boilerplate::RIGHT;
    else return;
    
    sigil_->bounds()->dump();

    if(dir == Boilerplate::LEFT && sigil_->bounds()->left() == 0)
        return;
    if(dir == Boilerplate::RIGHT && sigil_->bounds()->right() == dimensions_.x)
        return;
    sigil_->move(dir);
    sigil_->update(0.f,0.f);

}