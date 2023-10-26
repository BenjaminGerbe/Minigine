#pragma once

#include <vector>
#include <stack>
#include <iostream>

//libs
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>

#include "Object.h"
#include "../Headers/GLShader.h"
#include "Light.h"
#include "Camera.h"

enum SceneFlags{
    s_Grid = 1<<0,
    s_Grid_None = 1 << 1,
    WireFrame = 1 << 2,
    WireFrame_None = 1 << 3
};

class Scene{
    protected:
    int width;
    int height;
    std::vector<Object*> Objects;
    std::vector<LightComp*> Lights;
    std::vector<CameraComp*> Cameras;
    unsigned int VAO,UBO;
    unsigned int VAOS;
    uint32_t programShader;
    uint32_t programShaderWireFrame;
    uint32_t programShaderGrid;
   
    public:
    Scene(){
    };

    Scene(const Scene& scene){
        this->height = scene.height;
        this->width = scene.width;

        std::vector<Object*> tmp;
        for (int i = 0; i < scene.Objects.size(); i++)
        {
            tmp.push_back(new Object(scene.Objects[i]->GetMesh(),scene.Objects[i]->GetName(),scene.Objects[i]->GetObjectType()));
        }

        Objects = tmp;
    }

    Scene operator=(const Scene& copy){
        this->height = copy.height;
        this->width = copy.width;

        std::vector<Object*> tmp;
        for (int i = 0; i < copy.Objects.size(); i++)
        {
            tmp.push_back(new Object(copy.Objects[i]->GetMesh(),copy.Objects[i]->GetName(),copy.Objects[i]->GetObjectType()));
        }

        for (int i = 0; i < Objects.size(); i++)
        {
            delete Objects[i];
        }

        Objects = tmp;
        return *this;
    }


    ~Scene(){
        for(auto p : Objects){
            delete p;
        }
    }

    void SetUp();

    void RemoveObjectScene(int idx);

    void RemoveObjectScene(Object* object);

    std::vector<LightComp*> GetLightComp(){
        return this->Lights;
    }

    void Scene::RemoveObjectScene(std::vector<Object*> objects);

    void AddLight(LightComp* light){
        Lights.push_back(light);
    };

    void AddObjectScene(Object* object){
         Objects.push_back(object);
    }

     void Render(glm::mat4* _MVP,int flags);

    std::vector<Object*> GetObjects(){
        return this->Objects;
    }

    std::vector<CameraComp*> GetCameras(){
        return this->Cameras;
    }

    void AddCamera(CameraComp* cam){
        Cameras.push_back(cam);
    }

    int VertexToRender(){
        return 0;
    }
};


