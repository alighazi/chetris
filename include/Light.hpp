#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

using namespace glm;

class Light{
    protected:
    Light( vec3 ambient, vec3 diffuse, vec3 specular):_ambient(ambient), _diffuse(diffuse), _specular(specular)
    {}
    void addShaderUniform(GLuint program, std::string varName, std::string memberName, vec3 vec){
        glUniform3f(glGetUniformLocation(program, (varName+"."+memberName).c_str()), vec.x,vec.y,vec.z);        
    }
    void addShaderUniform(GLuint program, std::string varName, std::string memberName, float f){
        glUniform1f(glGetUniformLocation(program, (varName+"."+memberName).c_str()), f);        
    }

    public:
    virtual void addToShader(GLuint program, std::string varName){
        addShaderUniform(program, varName, "ambient", _ambient);
        addShaderUniform(program, varName, "diffuse", _diffuse);
        addShaderUniform(program, varName, "specular", _specular);
    }
    private:
    vec3 _ambient;
    vec3 _diffuse;
    vec3 _specular;
};

class DirectionalLight: Light{    
    public:
    DirectionalLight(vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular):Light(ambient, diffuse, specular),
    _direction(direction)
    {}   

    void addToShader(GLuint program, std::string varName) override {
        Light::addToShader(program, varName);
        glUniform3f(glGetUniformLocation(program, ("direction_"+varName).c_str()), _direction.x,_direction.y,_direction.z);  
    }
    
    private:
    vec3 _direction;
};

class PointLight:Light{
    public: 
    PointLight(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, float constant, float linear, float quadratic)
    :Light(ambient,diffuse,specular), _position(position), _constant(constant),_linear(linear), _quadratic(quadratic)
    {}

    void addToShader(GLuint program, std::string varName) override {
        Light::addToShader(program, varName);

        //postion is sent outside of the light struct cos they need to be converted to tangent space in vertex shader first
        glUniform3f(glGetUniformLocation(program, ("position_"+varName).c_str()), _position.x,_position.y,_position.z);   
        addShaderUniform(program, varName, "constant", _constant);        
        addShaderUniform(program, varName, "linear", _linear);        
        addShaderUniform(program, varName, "quadratic", _quadratic);   
    }

    private:
    vec3 _position;
    
    float _constant;
    float _linear;
    float _quadratic;  
};

class SpotLight:Light{
    public:
    SpotLight(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, vec3 direction, float constant, float linear, float quadratic, float cutoff, float outerCutoff)
    :Light(ambient,diffuse,specular),
    _position(position), _direction(direction), _constant(constant),_linear(linear), _quadratic(quadratic), _cutoff(cutoff), _outer_cutoff(outerCutoff)
    {}

    void addToShader(GLuint program, std::string varName) override {
        Light::addToShader(program, varName);

        //postion is sent outside of the light struct cos they need to be converted to tangent space in vertex shader first
        glUniform3f(glGetUniformLocation(program, ("position_"+varName).c_str()), _position.x,_position.y,_position.z);  
        glUniform3f(glGetUniformLocation(program, ("direction_"+varName).c_str()), _direction.x,_direction.y,_direction.z);  
        addShaderUniform(program, varName, "constant", _constant);        
        addShaderUniform(program, varName, "linear", _linear);        
        addShaderUniform(program, varName, "quadratic", _quadratic);   
        addShaderUniform(program, varName, "cutOff", _cutoff);     
        addShaderUniform(program, varName, "outerCutOff", _outer_cutoff);     
    }

    inline void setPosition(vec3 position){ _position=position; }
    inline void setDirection(vec3 direction){ _direction=direction; }

    private:
    vec3 _position;
    vec3 _direction;

    float _cutoff;
    float _outer_cutoff;

    float _constant;
    float _linear;
    float _quadratic;  
};

#endif