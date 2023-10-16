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
    public :
    RenderContextDisplay(){
        p_open = true;
    }

    RenderContextDisplay(const RenderContextDisplay& copy){
        this->p_open = copy.p_open;
        this->idx_renderContext = copy.idx_renderContext;
        this->renderContextes = std::vector<RenderContext*>(copy.renderContextes);
    }

    RenderContextDisplay operator=(const RenderContextDisplay& copy){
        this->p_open = copy.p_open;
        this->idx_renderContext = copy.idx_renderContext;
        this->renderContextes = std::vector<RenderContext*>(copy.renderContextes);
        return *this;
    }

    ~RenderContextDisplay(){
    }

    bool GetOpen(){
        return this->p_open;
    } 

    void AddRender(RenderContext* render){
        renderContextes.push_back(render);
    }

    void DisplayRenderWindow(int width,int height,Scene* scene,char* label);
};