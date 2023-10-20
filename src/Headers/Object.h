#pragma once
#include "Mesh.h"

#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>
#include <string>  

class Object{

    Mesh* mesh;
    glm::mat4 transformation;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    uint32_t programShader;
    std::string name;
    protected :

    public:

    Object(Mesh* _mesh,std::string _name):mesh(_mesh),name(_name){
        transformation = glm::mat4(1.0f);
        rotation = glm::vec3({0.0f,0.0f,0.0f});
        position = glm::vec3({0.0f,0.0f,0.0f});
        scale = glm::vec3({1.0f,1.0f,1.0f});
    }

    Mesh* GetMesh()const{
        return mesh;
    };

    ~Object(){
    }

    char* GetName(){
        char buffer[32];
        strcpy(buffer,name.c_str());
        return buffer;
    }

    std::string GetStrName(){
        return this->name;
    }

    glm::vec3 GetPosition(){
        return position;
    }

    glm::vec3 GetRotation(){
        return rotation;
    }

    glm::vec3 GetScale(){
        return scale;
    }

    void SetName(std::string str){
        name = str;
    }

    void SetPosition(glm::vec3 position){
        this->position = position;
    }

    void SetRotation(glm::vec3 rotation){
        this->rotation = rotation;
    }

    void SetScale(glm::vec3 scale){
        this->scale = scale;
    }

    glm::mat4 GetTransformation(){
        
        transformation = glm::mat4(1.0f);
        
     
        transformation *= glm::translate(glm::mat4(1.0f),position);
        transformation *= glm::rotate(glm::mat4(1.0f), rotation.x * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        transformation *= glm::rotate(glm::mat4(1.0f), rotation.y * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformation *= glm::rotate(glm::mat4(1.0f), rotation.z * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transformation *= glm::scale(glm::mat4(1.0f),scale);  

        return transformation;
    };

};

