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

    Transform(Object* obj):Component(obj){
        headerName = "Transformation##transform";
        SetID();
    }

    Transform(const Transform& copy):Component(copy){

    }

    virtual void SetID(){
        ID = c_Transform;
    }

    Transform operator=(const Transform& copy){
        return *this;
    }
};