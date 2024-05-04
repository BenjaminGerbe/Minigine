#pragma once
#include "Component.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Object.h"
    
class MaterialComp: public Component{

    public :
    RTTI_DECLARATION(MaterialComp)
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();

    MaterialComp(Object* obj):Component(obj){
        headerName = "Material";
        SetID();
    }

    MaterialComp(const MaterialComp& copy):Component(copy){
        headerName = "Material";
        SetID();
    }

    virtual MaterialComp* Clone(Object* obj){
        MaterialComp* comp = new MaterialComp(obj);
        return comp;
    }

    MaterialComp(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        this->headerName = "Material";
    }

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);
    }

    virtual void SetID(){
        ID = c_Material;
    }

    MaterialComp operator=(const MaterialComp& copy){
        return *this;

    }
};