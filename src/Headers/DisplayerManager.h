#pragma once
#include <vector>
#include <string>
#include "RenderContextDisplay.h"
#include <stdio.h>
#include <cstring>
#include <string> 
#include <algorithm>
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
    bool openSceneEditor;
    bool openObjectView;
    bool openSceneViewOption;
    int selectedObjects;
    int selectedSceneView;

    public :
    DisplayerManager(){
        selectedSceneView = -1;
        memoryUsage = std::vector<float>(500);
        openMachineState = true;
        openObjectView = true;
        openSceneEditor = true;
        openSceneViewOption = true;
    }
    
    void ObjectEditor(Scene* scene);

    void AddRenderContextDisplay(RenderContextDisplay* renderWindow);
    void SceneViewParameter();
    
    void MachineState();
    void SceneEditor(Scene* scene,std::vector<Object*> objets);
    void RenderAppOptions(Projet* projet);
    void RenderAllRenderWindows(int width,int height,Projet* projet);
    void RenderSceneViewOption();

    ~DisplayerManager(){
        for (size_t i = 0; i < RenderContextDisplays.size(); i++) {
            delete RenderContextDisplays[i];
        }
    }

  

};