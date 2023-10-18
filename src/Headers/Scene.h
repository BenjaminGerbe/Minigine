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


class Scene{
    protected:
    int width;
    int height;
    std::vector<Object*> Objects;
   
    public:
    Scene() = default;

    virtual void SetUp(){
    }

    virtual void AddObjectScene(Object* object){
        Objects.push_back(object);
    }

    virtual void Render(glm::mat4* _MVP,int debug){
    }

    virtual std::vector<Object*> GetObjects(){
        return this->Objects;
    }

    virtual int VertexToRender(){
        return 0;
    }
};

class Scene1 : public Scene{
    unsigned int VAO,UBO;
    unsigned int VAOS;
    uint32_t programShader;
    uint32_t programShaderWireFrame;
    uint32_t programShaderGrid;
    
    public :
    virtual void SetUp(int width,int height);
    float i = 0;
    virtual void Render(glm::mat4* _MVP,int debug);
};

