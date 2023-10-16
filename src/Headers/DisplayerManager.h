#pragma once

#include "RenderContextDisplay.h"

class Scene;

class DisplayerManager{
    std::vector<RenderContextDisplay*> RenderContextDisplays;
    public :
    DisplayerManager() = default;

    void AddRenderWindow(RenderContextDisplay* renderWindow){
        RenderContextDisplays.push_back(renderWindow);
    };

    void RenderAllRenderWindows(int width,int height,Scene* scene);

    ~DisplayerManager(){
        for (size_t i = 0; i < RenderContextDisplays.size(); i++) {
            delete RenderContextDisplays[i];
        }
    }

  

};