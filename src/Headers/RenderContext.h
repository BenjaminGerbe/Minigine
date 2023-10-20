// this class Specify a render context which a specific Scene can be Render, 
// we can specify camera, and some polygone render specic ou things like that, 
// /!\ so don't specify glDrawElements or things like that ine the scene, it must be done here

#pragma once
// glm include
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>
#include <cassert>
#include <string>
#include <iostream>

#include "../Headers/Scene.h"

enum RenderType{
    Orthographic,
    Perspective
};

class RenderContext{

    protected: 

    unsigned int FBO;
    unsigned int RBO;
    unsigned int Tex;
    int height;
    int width;
    char* label;
    glm::vec3 cameraPosition;


    public :
    RenderContext() = default;

    RenderContext(const RenderContext& render){
        FBO = render.FBO;
        Tex = render.Tex;
        height = render.height;
        width = render.width;
        label = render.label;
        cameraPosition = render.cameraPosition;
    }

    RenderContext operator=(const RenderContext& render){
        FBO = render.FBO;
        Tex = render.Tex;
        height = render.height;
        width = render.width;
        label = render.label;
        cameraPosition = render.cameraPosition;

        return *this;
    }

    virtual ~RenderContext(){
        glDeleteTextures(1,&Tex);
        glDeleteFramebuffers(1,&FBO);
        glDeleteRenderbuffers(1,&RBO);
    }

    virtual char* GetLabel(){ return label; }
    virtual void SetLabel(char* label){ this->label = label;};
    virtual void UpdateRender(int width,int height);
    virtual void SetUp(int width,int height);
    virtual int GetWidth();
    virtual int GetHeight();
    virtual unsigned int RenderScene(glm::mat4* MVP,Scene* scene);

};

class RenderContextNOFramed : public RenderContext{
    public :
    virtual void SetUp(int width,int height);
    virtual unsigned int RenderScene(glm::mat4* MVP,Scene* scene);

    RenderContextNOFramed() = default;
    RenderContextNOFramed(const RenderContextNOFramed& render){
        FBO = render.FBO;
        Tex = render.Tex;
        height = render.height;
        width = render.width;
        label = render.label;
        cameraPosition = render.cameraPosition;
    }

    RenderContextNOFramed operator=(const RenderContextNOFramed& render){
        FBO = render.FBO;
        Tex = render.Tex;
        height = render.height;
        width = render.width;
        label = render.label;
        cameraPosition = render.cameraPosition;

        return *this;
    }

};

class RenderContextShaded : public RenderContext{
    public :
    virtual unsigned int RenderScene(glm::mat4* MVP,Scene* scene);

    RenderContextShaded() = default;
    RenderContextShaded(const RenderContextShaded& render){
        FBO = render.FBO;
        Tex = render.Tex;
        height = render.height;
        width = render.width;
        label = render.label;
        cameraPosition = render.cameraPosition;
    }

    RenderContextShaded operator=(const RenderContextShaded& render){
        FBO = render.FBO;
        Tex = render.Tex;
        height = render.height;
        width = render.width;
        label = render.label;
        cameraPosition = render.cameraPosition;

        return *this;
    }
};

class RenderContextWireFrame: public RenderContext{
    public :
    virtual unsigned int RenderScene(glm::mat4* MVP,Scene* scene);

    RenderContextWireFrame() = default;
    RenderContextWireFrame(const RenderContextWireFrame& render){
        FBO = render.FBO;
        Tex = render.Tex;
        height = render.height;
        width = render.width;
        label = render.label;
        cameraPosition = render.cameraPosition;
    }

    RenderContextWireFrame operator=(const RenderContextWireFrame& render){
        FBO = render.FBO;
        Tex = render.Tex;
        height = render.height;
        width = render.width;
        label = render.label;
        cameraPosition = render.cameraPosition;

        return *this;
    }
    
};

class RenderContextShadedWireFrame : public RenderContext{
    public :

    unsigned int RenderScene(glm::mat4* MVP,Scene* scene);

    RenderContextShadedWireFrame() = default;
    RenderContextShadedWireFrame(const RenderContextShadedWireFrame& render){
        FBO = render.FBO;
        Tex = render.Tex;
        height = render.height;
        width = render.width;
        label = render.label;
        cameraPosition = render.cameraPosition;
    }

    RenderContextShadedWireFrame operator=(const RenderContextShadedWireFrame& render){
        FBO = render.FBO;
        Tex = render.Tex;
        height = render.height;
        width = render.width;
        label = render.label;
        cameraPosition = render.cameraPosition;

        return *this;
    }
};
    