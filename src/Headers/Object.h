#pragma once
#include "Mesh.h"

#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>
#include <string>  
#include <vector>

class Component;

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
    std::vector<Component*> components;

    public:

    Object() = default;

    Object(Mesh* _mesh,std::string _name,ObjectType _objectType):mesh(_mesh),name(_name){
        transformation = glm::mat4(1.0f);
        rotation = glm::vec3({0.0f,0.0f,0.0f});
        position = glm::vec3({0.0f,0.0f,0.0f});
        scale = glm::vec3({1.0f,1.0f,1.0f});
        objectType = _objectType;
        AddTransform();
    
    }

    Object(const Object& copy){
        this->mesh = copy.mesh;
        this->name = copy.name;
        this->objectType = copy.objectType;
       transformation = glm::mat4(1.0f);
        rotation = glm::vec3({0.0f,0.0f,0.0f});
        position = glm::vec3({0.0f,0.0f,0.0f});
        scale = glm::vec3({1.0f,1.0f,1.0f});
        this->programShader = copy.programShader;

        CopyComponents(copy.components);
    }

    Object operator=(const Object& copy){
        this->mesh = copy.mesh;
        this->name = copy.name;
        this->objectType = copy.objectType;
        this->programShader = copy.programShader;

        CopyComponents(copy.components);
    }

    void DeleteComponents();
    void AddTransform();

    void CopyComponents(std::vector<Component*> copy);

    ~Object(){
        DeleteComponents();
    }

    std::vector<Component*> GetComponents(){
        return components;
    }

    char* GetName(){
        char buffer[32];
        strcpy(buffer,name.c_str());
        return buffer;
    }

    ObjectType GetObjectType(){
        return objectType;
    }

    Mesh* GetMesh()const{
        return mesh;
    };

    std::string GetStrName(){
        return this->name;
    }

    glm::vec3 GetPosition(){
        return position;
    }

    glm::vec3 GetRotation(){
        std::cout <<    rotation.y << std::endl;
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

    glm::mat4 GetTransformation();

};
