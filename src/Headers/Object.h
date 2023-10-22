#pragma once
#include "Mesh.h"

#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>
#include <string>  


enum ObjectType{
    ClassicObject,
    Light,
    Camera
};

class Object{

    protected :
    Mesh* mesh;
    glm::mat4 transformation;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    uint32_t programShader;
    std::string name;
    ObjectType objectType;

    public:

    Object() = default;

    Object(Mesh* _mesh,std::string _name,ObjectType _objectType):mesh(_mesh),name(_name){
        transformation = glm::mat4(1.0f);
        rotation = glm::vec3({0.0f,0.0f,0.0f});
        position = glm::vec3({0.0f,0.0f,0.0f});
        scale = glm::vec3({1.0f,1.0f,1.0f});
        objectType = _objectType;
    }

    ~Object(){
    }

    virtual char* GetName(){
        char buffer[32];
        strcpy(buffer,name.c_str());
        return buffer;
    }

    virtual ObjectType GetObjectType(){
        return objectType;
    }

    virtual Mesh* GetMesh()const{
        return mesh;
    };

    virtual std::string GetStrName(){
        return this->name;
    }

    virtual glm::vec3 GetPosition(){
        return position;
    }

    virtual glm::vec3 GetRotation(){
        return rotation;
    }

    virtual glm::vec3 GetScale(){
        return scale;
    }

    virtual void SetName(std::string str){
        name = str;
    }

    virtual void SetPosition(glm::vec3 position){
        this->position = position;
    }

    virtual void SetRotation(glm::vec3 rotation){
        this->rotation = rotation;
    }

    virtual void SetScale(glm::vec3 scale){
        this->scale = scale;
    }

    virtual glm::mat4 GetTransformation(){
        
        transformation = glm::mat4(1.0f);
        
        transformation *= glm::translate(glm::mat4(1.0f),position);
        transformation *= glm::rotate(glm::mat4(1.0f), rotation.x * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        transformation *= glm::rotate(glm::mat4(1.0f), rotation.y * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformation *= glm::rotate(glm::mat4(1.0f), rotation.z * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transformation *= glm::scale(glm::mat4(1.0f),scale);  

        return transformation;
    };

};
