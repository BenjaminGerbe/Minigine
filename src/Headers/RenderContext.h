// RenderContext is in charge to create Texture and FBO, it will bind the correct FBO and call glPolyMode to have
// wireframe or shaded for exemple

#pragma once
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
    Perspective,
    Orthographic
};

class RenderContext{

    protected: 
    GLuint FBO;
    GLuint RBO;
    GLuint Tex;
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

    RenderContextNOFramed():RenderContext(){
    };

    RenderContextNOFramed(const RenderContextNOFramed& render):RenderContext(render){
    }

    RenderContextNOFramed operator=(const RenderContextNOFramed& render){
        RenderContext::operator=(render);
        return *this;
    }

};

class RenderContextShaded : public RenderContext{
    public :
    virtual unsigned int RenderScene(glm::mat4* MVP,Scene* scene);

    RenderContextShaded():RenderContext(){

    };
    RenderContextShaded(const RenderContextShaded& render):RenderContext(render){
    }

    RenderContextShaded operator=(const RenderContextShaded& render){
        RenderContext::operator=(render);
        return *this;
    }
};

class RenderContextGame: public RenderContext{
    public :
    virtual unsigned int RenderScene(glm::mat4* MVP,Scene* scene);

    RenderContextGame():RenderContext(){

    };
    RenderContextGame(const RenderContextGame& render):RenderContext(render){
    }

    RenderContextGame operator=(const RenderContextGame& render){
        RenderContext::operator=(render);
        return *this;
    }
};


class RenderContextWireFrame: public RenderContext{
    public :
    virtual unsigned int RenderScene(glm::mat4* MVP,Scene* scene);

    RenderContextWireFrame():RenderContext(){

    };
    RenderContextWireFrame(const RenderContextWireFrame& render):RenderContext(render){
        
    }

    RenderContextWireFrame operator=(const RenderContextWireFrame& render){
        RenderContext::operator=(render);
        return *this;
    }
    
};

class RenderContextShadedWireFrame : public RenderContext{
    public :

    unsigned int RenderScene(glm::mat4* MVP,Scene* scene);

    RenderContextShadedWireFrame():RenderContext(){
    };
    RenderContextShadedWireFrame(const RenderContextShadedWireFrame& render):RenderContext(render){
    }

    RenderContextShadedWireFrame operator=(const RenderContextShadedWireFrame& render){
        RenderContext::operator=(render);
        return *this;
    }
};
    