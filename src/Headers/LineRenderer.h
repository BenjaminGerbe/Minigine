#pragma once
#include "Component.h"
#include "Projet.h"
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>


class LineRenderer:public Component{
    Object* targetMouse;
    Projet* projet;
    public :
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();

    LineRenderer(Object* obj):Component(obj){
        headerName = "LineRenderer";
        SetID();
    }

    LineRenderer(const LineRenderer& copy):Component(copy){

    }

    LineRenderer(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        headerName = "LineRenderer";
    }

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);
    }

    virtual void SetID(){
        ID = c_LineRenderer;
    }

    LineRenderer operator=(const LineRenderer& copy){
        return *this;
    }

    ~LineRenderer(){
       // delete targetMouse;
    }
};