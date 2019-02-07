#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "core/shape.h"
#include "core/sigil.h"
#include "core/util/to_string.hpp"

Sigil::Sigil(const bool blocks[SIZE][SIZE], const glm::ivec2 pos){
    vector<Vertex> cube = shape::cube_vertex_array();
    glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(SCALE, SCALE, 1.f));

    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            blocks_[i][j] = blocks[i][j];
            if(blocks[i][j]){
                glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(j+0.5f,i+0.5f,0.f));
                for(int k=0; k < cube.size(); k++){
                    Vertex v = Vertex(scale * model * glm::vec4(cube[k].position,1.f) , cube[k].color, cube[k].tex_coords);
                    vertices_.push_back(v);
                }
            }
        }
    }
    bounds_ = iRect(pos, width(), height());
 
    //TODO: populate indices as well
    auto indices = std::vector<unsigned int>();

    glGenVertexArrays(1, &vAO_);
    glGenBuffers(1, &vBO_);
    glGenBuffers(1, &eBO_);

    //initializing VBO, VAO
    glBindVertexArray(vAO_);
    glBindBuffer(GL_ARRAY_BUFFER, vBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size()*sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);

    //Element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    // texture cordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
    update(0.f,0.f);
}

void Sigil::render(Shader* shader){
    glBindVertexArray(vAO_);
    shader->setMat4("model", transform_.transform());
    glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
}

void Sigil::update(float dt, float t){
    bounds_.move(0, -int(glm::floor(dt)));
    transform_.setTransform(glm::translate(glm::mat4(1.f),
     glm::vec3(bounds_.position().x, bounds_.position().y, 0.f)));
}

Sigil::~Sigil(){
    glDeleteVertexArrays(1, &vAO_);
    glDeleteBuffers(1, &vBO_);
    glDeleteBuffers(1, &eBO_);
}

int Sigil::width(){
    int w = 0;
    for(int i = 0; i < SIZE; i++)
    {
        for(int j = 0; j < SIZE; j++)
        {
            if(blocks_[i][j] && j+1 > w){
                w = j+1;
            }
        }
    }
    w*=SCALE;
    return w;
}

int Sigil::height(){
    int h=0;
    for(int i = 0; i < SIZE; i++)
    {
        for(int j = 0; j < SIZE; j++)
        {
            if(blocks_[i][j]){
                h = i+1;
                continue;
            }
        }
    }
    h*=SCALE;
    return h;
}

void Sigil::move(Boilerplate::Direction dir){
    if(dir & Boilerplate::LEFT){
        bounds_.move(-SCALE, 0);
        std::cout<<"move left"<<std::endl;
    }
    else if (dir & Boilerplate::RIGHT){
        bounds_.move(+SCALE, 0);
        std::cout<<"move right"<<std::endl;
    }
}