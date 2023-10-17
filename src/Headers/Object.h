#pragma once
#include "Mesh.h"

#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>

class Object{
    Mesh* mesh;
    glm::mat4 transformation;
    uint32_t programShader;
    char* name;

    public:

    Object(Mesh* _mesh,char* _name):mesh(_mesh),name(_name){
        transformation = glm::mat4(1.0f);
    }

    Mesh* GetMesh()const{
        return mesh;
    };

    const char* GetName(){
        return this->name;
    }

    void SetTransformation( glm::mat4 tr){
        transformation = tr;
    };

    glm::mat4 GetTransformation(){
        return transformation;
    };

};