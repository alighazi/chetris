#include <vector>
#include <glm/glm.hpp>
using std::vector;
using glm::vec3;

namespace triangle_fractal{
vector<vec3> inner_triangle(vec3 &a,vec3 &b,vec3 &c, int depth){
    float z= -depth/100.f;
    auto in_a = (c-b)/2.f+b;
    auto in_b = (a-c)/2.f+c;
    auto in_c = (b-a)/2.f+a;
    return  {
                vec3(in_a.x, in_a.y, z),
                vec3(in_b.x, in_b.y, z),
                vec3(in_c.x, in_c.y, z)
            };
}

vector<vec3> fractal(vec3 &a,vec3 &b,vec3 &c, int depth){
    if(depth==0) return vector<vec3>();

    auto inner = inner_triangle(a,b,c, depth);
    if(depth==1)
        return inner;
    
    depth--;
    auto vertices = vector<vec3>();
    vec3& inner_a=inner[0];
    vec3& inner_b=inner[1];
    vec3& inner_c=inner[2];
    auto top_triangle = fractal(inner_b, inner_a , c, depth);
    auto left_triangle = fractal(a, inner_c , inner_b, depth);
    auto right_triangle = fractal(inner_c, b , inner_a, depth);

    vertices.insert(std::end(vertices), std::begin(top_triangle), std::end(top_triangle));
    vertices.insert(std::end(vertices), std::begin(right_triangle), std::end(right_triangle));
    vertices.insert(std::end(vertices), std::begin(left_triangle), std::end(left_triangle));
    vertices.insert(std::end(vertices), std::begin(inner), std::end(inner));

    return vertices;
}

vector<float> compute_vertices(int depth, int color_seed){
    auto vertices = vector<float>();
    int stride=8;
    vec3 a = vec3(-1.f, -1.f, 0.0f);
    vec3 b = vec3(1.f, -1.f, 0.0f);
    vec3 c = vec3(0.f,  1.f, 0.0f);
    auto inner=fractal(a,b,c, depth);
    for(int i=0;i<inner.size();i++){
        vertices.push_back(inner[i].x);
        vertices.push_back(inner[i].y);
        vertices.push_back(inner[i].z);
        //color
        int j=i+color_seed;
        float rad1=glm::radians((j%4) * 90.f);
        float rad2=glm::radians((j%16) * 22.5f);
        float rad3=glm::radians((j%36) * 10.f);
        vertices.push_back(0.5*sin(rad1)+0.5f);
        vertices.push_back(0.5*sin(rad2)+0.5f);
        vertices.push_back(0.5*sin(rad3)+0.5f);
        //u,v
        if(i%3 == 0){
            //v= a = top right
            vertices.push_back(-0.5f);
            vertices.push_back(1.2f);
        }else if (i%3 == 1){
            //v= b = top left
            vertices.push_back(1.5f);
            vertices.push_back(1.2f);
        }else{
            //v = c = bottom mid
            vertices.push_back(0.5f);
            vertices.push_back(-0.7f);
        }
    }
    return vertices;
}

}//namepsace