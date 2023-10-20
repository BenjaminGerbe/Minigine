#pragma once
#include<vector>

// imgui include
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "RenderContext.h"

class Scene;

class RenderContextDisplay{

    bool p_open;
    int idx_renderContext;
    std::vector<RenderContext*> renderContextes;
    char* label;
    glm::mat4* MVP;
    public :
    RenderContextDisplay(){
        idx_renderContext= 0;
        p_open = true;
        MVP = new glm::mat4[2];
        MVP[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, .6f,5.f }));
        MVP[0] *= glm::rotate(glm::mat4(1.0f), 15 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
        MVP[0] *= glm::rotate(glm::mat4(1.0f), 15 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
        MVP[1] = glm::mat4(1.0f);
    }

    RenderContextDisplay(const RenderContextDisplay& copy){
        this->p_open = copy.p_open;
        this->idx_renderContext = copy.idx_renderContext;
        this->renderContextes = std::vector<RenderContext*>(copy.renderContextes);

        MVP = new glm::mat4[2];
        MVP[0] = copy.MVP[0];
        MVP[1] =  copy.MVP[1];
    }

    std::vector<RenderContext*> getRenderContextes(){
        return this->renderContextes;
    }

    RenderContextDisplay operator=(const RenderContextDisplay& copy){
        this->p_open = copy.p_open;
        this->idx_renderContext = copy.idx_renderContext;
        this->renderContextes = std::vector<RenderContext*>(copy.renderContextes);

        MVP = new glm::mat4[2];
        MVP[0] = copy.MVP[0];
        MVP[1] =  copy.MVP[1];
        return *this;
    }

    ~RenderContextDisplay(){
        for(auto p : renderContextes){
            delete p;
        }

        delete[] MVP;
    }

    bool GetOpen(){
        return this->p_open;
    } 

    void AddRender(RenderContext* render){
        renderContextes.push_back(render);
    }

    void DisplayRenderWindow(int width,int height,Scene* scene,char* label);
};