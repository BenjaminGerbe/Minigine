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

class Scene;

class RenderContext{

    protected: 

    unsigned int UBO;
    unsigned int FBO;
    unsigned int Tex;
    int height;
    int width;

    glm::mat4* MVP;
    glm::vec3 cameraPosition;

    public :
    RenderContext()=default;
    virtual void SetUp(glm::mat4* _MVP,int width,int height);
    virtual int GetWidth();
    virtual int GetHeight();
    virtual unsigned int RenderScene(Scene* scene);

};

class RenderContextNOFramed : public RenderContext{
    public :
    virtual void SetUp(glm::mat4* _MVP,int width,int height);
    virtual unsigned int RenderScene(Scene* scene);
};

class RenderContextShaded : public RenderContext{
    public :
    virtual unsigned int RenderScene(Scene* scene);
};

class RenderContextWireFrame: public RenderContext{
    public :
    virtual unsigned int RenderScene(Scene* scene);
};

class RenderContextShadedWireFrame : public RenderContext{
    public :
    unsigned int RenderScene(Scene* scene);
};
    