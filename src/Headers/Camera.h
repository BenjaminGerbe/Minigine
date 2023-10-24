#pragma once
#include "Component.h"

// glm include
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>

class CameraComp : public Component{
    public :
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();

    CameraComp(Object* obj):Component(obj){
        headerName = "Camera";
        SetID();
    }

    CameraComp(const CameraComp& copy):Component(copy){
        
    }

    CameraComp(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        headerName = "Camera";
    }

    glm::mat4* GetTransform(){
        this->obj->GetTransformation();
    }

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);
    }

    virtual void SetID(){
        ID = c_Camera;
    }

    CameraComp operator=(const CameraComp& copy){
        return *this;
    }
};