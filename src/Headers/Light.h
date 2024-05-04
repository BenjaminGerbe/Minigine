#pragma once
#include "Component.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Object.h"


class LightComp:public Component{
    private:
    float* color;
    float intensity;
    public :
    RTTI_DECLARATION(LightComp)
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();

    LightComp(Object* obj):Component(obj){
        headerName = "Light";
        SetID();
        color = new float[3];
        intensity = 1.0f;
    }

    virtual LightComp* Clone(Object* obj){
        LightComp* comp = new LightComp(obj);
        return comp;
    }

    LightComp(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        this->headerName = "Light";
        color = new float[3];
        color[0] = yamlFile[id][i]["color"][0].as<float>();
        color[1] = yamlFile[id][i]["color"][1].as<float>();
        color[2] = yamlFile[id][i]["color"][2].as<float>();

        this->intensity = yamlFile[id][i]["intensity"].as<float>();
    }

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);
        yamlFile[id][i]["color"].push_back(color[0]);
        yamlFile[id][i]["color"].push_back(color[1]);
        yamlFile[id][i]["color"].push_back(color[2]);
        yamlFile[id][i]["intensity"] = intensity;
    }

    LightComp(const LightComp& copy):Component(copy){

    }

    float* GetColor(){
        return color;
    }

    float GetIntensity(){
        return intensity;
    }

    ~LightComp();

    virtual void SetID(){
        ID = c_Light;
    }

    LightComp operator=(const LightComp& copy){
        return *this;
    }
};