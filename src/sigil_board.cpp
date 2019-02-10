#include "core/sigil_board.h"
#include "core/boilerplate.h"

SigilBoard::SigilBoard(int width, int height)
{
    dimensions_= glm::vec2(width,height);
    Boilerplate::instance().addInputHandler(this);  
    spawn();
}

SigilBoard::~SigilBoard()
{
    for(Sigil* s:all_){
        delete s;
    }
    all_.clear();
    sigil_ = nullptr;
}

void SigilBoard::render(Shader* shader){
    for(Sigil* s:all_){
        s->render(shader);
    }
}

void SigilBoard::update(float dt, float t){    
    static float last_frame=0.f;
    last_frame += dt;
    if(last_frame < 1.f)
        return;

    sigil_->move(Boilerplate::DOWN);
    if(sigil_->bounds()->bottom() <0 || checkCollision()){
        sigil_->move(Boilerplate::UP);
        spawn();
    }
    for(Sigil* s:all_){
        s->update(dt, t);
    }
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

void SigilBoard::spawn(){
    using type = const block_def;
    static type block_types[] = {  Sigil::blocks_S, Sigil::blocks_SR,
                                    Sigil::blocks_L, Sigil::blocks_LR,
                                    Sigil::blocks_Box, Sigil::blocks_Bar};
    int r = std::rand()% 6;
    std::cout<<"spawning: "<<r<<std::endl;

    sigil_ =  new Sigil(block_types[r], vec3(0.f, (float)dimensions_.y- 3, 1.f));
    all_.push_back(sigil_);
}

bool SigilBoard::checkCollision(){
    for(int i=0;i<all_.size(); i++){
        if(all_[i]!=sigil_ && sigil_->collidesWith(all_[i])){
            std::cout<<"collision occured"<<std::endl;
            return true;
        }
    }
}
