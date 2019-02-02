#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "fmt/core.h"

namespace std{

    string to_string(const glm::vec3 &v){
        return fmt::format("[{:.4}, {:.4}, {:.4}]", v.x,v.y,v.z);
    }
    string to_string(const glm::vec2 &v){
        return fmt::format("[{:.4}, {:.4}]", v.x,v.y);
    }

    string to_string(std::vector<glm::vec3> &v){
        string s="";
        for(int i=0;i<v.size();i++){
            if(i>0) s+="\n";
            s+= fmt::format("{}/{}=> [{:.4}, {:.4}, {:.4}]", i+1,v.size(), v[i].x,v[i].y,v[i].z);
        }
        return s;
    }

    template<class T>
    string to_string_vscalar(std::vector<T> &v){
        string s=fmt::format("{}:[",v.size());
        for(int i=0;i<v.size();i++){
            if(i>0) s+=", ";
            s+=fmt::format("{}",v[i]);
        }
        return s+"]";
    }

    template<class T>
    string to_string_vector_float(std::vector<T> &v, int precision){
        string s=fmt::format("{}:[",v.size());
        for(int i=0;i<v.size();i++){
            if(i>0) s+=", ";
            s+=fmt::format("{:.{}f}",v[i], precision);
        }
        return s+"]";
    }
    
    string to_string(std::vector<unsigned int> &v){
        return to_string_vscalar(v);
    }
    string to_string(std::vector<int> &v){
        return to_string_vscalar(v);
    }
    string to_string(std::vector<char> &v){
        return to_string_vscalar(v);
    }
    string to_string(std::vector<long> &v){
        return to_string_vscalar(v);
    }
    string to_string(std::vector<float> &v, int precision = 4){
        return to_string_vector_float(v, precision);
    }
    string to_string(std::vector<double> &v, int precision = 8){
        return to_string_vector_float(v, precision);
    }

}