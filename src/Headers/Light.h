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
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();

    LightComp(Object* obj):Component(obj){
        headerName = "Light##transform";
        SetID();
        color = new float[3];
        intensity = 1.0f;
    }

    LightComp(const LightComp& copy):Component(copy){

    }

    float* GetColor(){
        return color;
    }

    float GetIntensity(){
        return intensity;
    }

    ~LightComp(){
        delete color;
    }

    virtual void SetID(){
        ID = c_Light;
    }

    LightComp operator=(const LightComp& copy){
        return *this;
    }
};