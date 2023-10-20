#pragma once
#include <vector>
#include <string>
#include "RenderContextDisplay.h"
#include <stdio.h>
#include <cstring>
#include <string> 
#include "windows.h"
#include "psapi.h"
#include "Scene.h"
#include "Mesh.h"
#include "Saver.h"

class DisplayerManager{
    std::vector<RenderContextDisplay*> RenderContextDisplays;
    RenderContextShadedWireFrame rcWireShadedFrame;
    RenderContextShaded rcShaded;
    RenderContextWireFrame rcWireFrame;
    RenderContextDisplay* renderContextDisplay;
    std::vector<float> memoryUsage;
    bool openMachineState;
    int selectedObjects;

    public :
    DisplayerManager(){
        memoryUsage = std::vector<float>(500);
        openMachineState = true;
    }
    
    void ObjectEditor(Scene* scene);

    void AddRenderContextDisplay(RenderContextDisplay* renderWindow){
        RenderContextDisplays.push_back(renderWindow);

        renderWindow->AddRender(new RenderContextShaded());
        renderWindow->getRenderContextes()[0]->SetLabel("Shaded");
        renderWindow->AddRender(new RenderContextShadedWireFrame());
        renderWindow->getRenderContextes()[1]->SetLabel("ShadedWireFrame");
        renderWindow->AddRender(new RenderContextWireFrame());
        renderWindow->getRenderContextes()[2]->SetLabel("WireFrame");

        std::cout << " je t'en suplie change ca " << std::endl;
        for (int i = 0; i < renderWindow->getRenderContextes().size(); i++)
        {
            renderWindow->getRenderContextes()[i]->SetUp(1920,1080);
        }
        

    };

    void MachineState();
    void SceneEditor(Scene* scene,std::vector<Mesh*> objets);
    void RenderAppOptions(Projet* projet);
    void RenderAllRenderWindows(int width,int height,Scene* scene);


    ~DisplayerManager(){
        for (size_t i = 0; i < RenderContextDisplays.size(); i++) {
            delete RenderContextDisplays[i];
        }
    }

  

};