#pragma once
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 tex_coords;
    Vertex(glm::vec3 pos, glm::vec3 clr, glm::vec2 txcoords) : position(pos), color(clr), tex_coords(txcoords){}
};
