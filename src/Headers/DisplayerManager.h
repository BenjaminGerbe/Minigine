#pragma once

#include "RenderContextDisplay.h"

class Scene;

class DisplayerManager{
    std::vector<RenderContextDisplay*> RenderContextDisplays;
    RenderContextDisplay* renderContextDisplay;
    
    public :
    DisplayerManager(){
    }
    
    void AddRenderContextDisplay(RenderContextDisplay* renderWindow){
        RenderContextDisplays.push_back(renderWindow);
    };

    void SetTemplateRenderContextDisplay(RenderContextDisplay* render){
        renderContextDisplay = render;
    }

    void RenderAppOptions();
    void RenderAllRenderWindows(int width,int height,Scene* scene);


    ~DisplayerManager(){
        for (size_t i = 0; i < RenderContextDisplays.size(); i++) {
            delete RenderContextDisplays[i];
        }
    }

  

};