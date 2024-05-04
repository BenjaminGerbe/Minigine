#pragma once
#include "Component.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Object.h"
#include "Projet.h"

class MeshComponent: public Component{
    Projet* projet;

    public :
    RTTI_DECLARATION(MeshComponent)
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();

    MeshComponent(Object* obj):Component(obj){
        this->headerName = "Mesh";
        SetID();
    }

    MeshComponent(const MeshComponent& copy):Component(copy){
        this->headerName = "Mesh";
        SetID();
    }

    MeshComponent operator=(const MeshComponent& copy){
        return *this;

    }

    virtual MeshComponent* Clone(Object* obj){
        MeshComponent* comp = new MeshComponent(obj);
        return comp;
    }

    MeshComponent(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        this->headerName = "Mesh";
    }

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);

    }

    virtual void SetID(){
        ID = c_Mesh;
    }

};