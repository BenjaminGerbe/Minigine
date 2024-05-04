#pragma once
#include "Component.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Object.h"
    
class Transform: public Component{

    public :
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();
    RTTI_DECLARATION(Transform)
    Transform(Object* obj):Component(obj){
        headerName = "Transformation";
        SetID();
    }

    Transform(const Transform& copy):Component(copy){
        headerName = "Transformation";
        SetID();
    }

    virtual Transform* Clone(Object* obj){
        Transform* comp = new Transform(obj);
        return comp;
    }

    Transform(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        this->headerName = "Transform";
        
        double x = yamlFile[id][i]["Position"][0].as<double>();
        double y = yamlFile[id][i]["Position"][1].as<double>();
        double z = yamlFile[id][i]["Position"][2].as<double>();

        glm::vec3 position({x,y,z});

        x = yamlFile[id][i]["Rotation"][0].as<double>();
        y = yamlFile[id][i]["Rotation"][1].as<double>();
        z = yamlFile[id][i]["Rotation"][2].as<double>();

        glm::vec3 rotation({x,y,z});

        x = yamlFile[id][i]["Scale"][0].as<double>();
        y = yamlFile[id][i]["Scale"][1].as<double>();
        z = yamlFile[id][i]["Scale"][2].as<double>();

        glm::vec3 scale({x,y,z});

        this->obj->SetPosition(position);
        this->obj->SetRotation(rotation);
        this->obj->SetScale(scale);
    }

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);
        yamlFile[id][i]["Position"].push_back(obj->GetPosition().x);
        yamlFile[id][i]["Position"].push_back(obj->GetPosition().y);
        yamlFile[id][i]["Position"].push_back(obj->GetPosition().z);

        yamlFile[id][i]["Rotation"].push_back(obj->GetRotation().x);
        yamlFile[id][i]["Rotation"].push_back(obj->GetRotation().y);
        yamlFile[id][i]["Rotation"].push_back(obj->GetRotation().z);

        yamlFile[id][i]["Scale"].push_back(obj->GetScale().x);
        yamlFile[id][i]["Scale"].push_back(obj->GetScale().y);
        yamlFile[id][i]["Scale"].push_back(obj->GetScale().z);

    }

    virtual void SetID(){
        ID = c_Transform;
    }

    Transform operator=(const Transform& copy){
        return *this;

    }
};