#pragma once
#include "Component.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Object.h"
#include "Projet.h"
    
class WaterComp: public Component{

    Projet* projet;

    public :
    RTTI_DECLARATION(WaterComp)
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();

    WaterComp(Object* obj):Component(obj){
        headerName = "WaterComp";  
        SetID();
    }

    WaterComp(const WaterComp& copy):Component(copy){
        headerName = "WaterComp";
        SetID();
    }

    virtual WaterComp* Clone(Object* obj){
        WaterComp* comp = new WaterComp(obj);
        return comp;
    }

    WaterComp(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        this->headerName = "WaterComp";
    }

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);
    }

    virtual void SetID(){
        ID = c_Water;
    }

    WaterComp operator=(const WaterComp& copy){
        return *this;
    }
};