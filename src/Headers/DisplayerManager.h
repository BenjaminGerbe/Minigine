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
#include "Component.h"
#include "GameView.h"
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>

// imgui include
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

class DisplayerManager{
    std::vector<RenderContextDisplay*> RenderContextDisplays;
    RenderContextShadedWireFrame rcWireShadedFrame;
    RenderContextShaded rcShaded;
    RenderContextWireFrame rcWireFrame;
    RenderContextGame* rcGameView;
    RenderContextDisplay* renderContextDisplay;
    std::vector<float> memoryUsage;
    std::vector<float> fpsCounter;
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
        fpsCounter = std::vector<float>(25);
        openMachineState = true;
        openObjectView = true;
        openSceneEditor = true;
        openSceneViewOption = true;
        rcGameView = new RenderContextGame();
    }
    
    void ObjectEditor(Projet* projet);
    void AddRenderContextDisplay(RenderContextDisplay* renderWindow);
    void SceneViewParameter();
    void MachineState(Projet* projet);
    void SceneEditor(Projet* projet);
    void RenderAppOptions(Projet* projet);
    void RenderAllRenderWindows(int width,int height,Projet* projet);
    void RenderSceneViewOption();
    void RenderGameView(GameView* GameView,Projet* projet);

    ~DisplayerManager(){
        for (size_t i = 0; i < RenderContextDisplays.size(); i++) {
            delete RenderContextDisplays[i];
        }
    }

  

};