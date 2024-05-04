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

#include "Component.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class CameraComp : public Component{
    int RenderMode;
    glm::mat4* MVP;
    float fov;
    float size;
    public :
    RTTI_DECLARATION(CameraComp)
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();

    CameraComp(Object* obj):Component(obj){
        headerName = "Camera";
        RenderMode = 0;
        size = 5.0f;
        fov = 45.5f;
        MVP = new glm::mat4[2];
        SetID();
    }

    CameraComp(const CameraComp& copy):Component(copy){
        RenderMode = copy.RenderMode;
        MVP = new glm::mat4[2];
        MVP[0] = copy.MVP[0];
        MVP[0] = copy.MVP[1];
        fov = copy.fov;
        size = copy.size;
    }

    CameraComp(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        headerName = "Camera";
        MVP = new glm::mat4[2];
        RenderMode = yamlFile[id][i]["RenderMode"].as<int>();
        fov = yamlFile[id][i]["Fov"].as<float>();
        size = yamlFile[id][i]["Size"].as<float>();
    }

    
    virtual CameraComp* Clone(Object* obj){
        CameraComp* comp = new CameraComp(obj);
        return comp;
    }

    glm::mat4* GetMVP(float width,float height){
        this->obj->GetTransformation();
     
        this->MVP[0] = obj->GetTransformation();
        if(RenderMode == 0){
            this->MVP[1] = glm::perspectiveFov(glm::radians(fov), width, height, 0.1f, 1000.0f);
        }
        else{
            float ratio = width/height;
            float w =  ratio*size;
            float h =  size;
            MVP[1] =  glm::ortho( -w/2.0f, w/2.0f,-h/2.0f, h/2.0f, 0.1f,1000.0f);
        }

        return MVP;
    }

    ~CameraComp();

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);
        yamlFile[id][i]["RenderMode"] = RenderMode;
        yamlFile[id][i]["Fov"] = fov;
        yamlFile[id][i]["Size"] = size;
    }

    virtual void SetID(){
        ID = c_Camera;
    }

    CameraComp operator=(const CameraComp& copy){
        RenderMode = copy.RenderMode;
        MVP = new glm::mat4[2];
        MVP[0] = copy.MVP[0];
        MVP[0] = copy.MVP[1];
        fov = copy.fov;
        size = copy.size;
        return *this;
    }
};