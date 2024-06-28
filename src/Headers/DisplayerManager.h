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
#include <MiniML.hpp>

// imgui include
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "implot.h"
#include "MiniMLDisplay.h"
class Network;

class DisplayerManager{
    std::vector<RenderContextDisplay*> RenderContextDisplays;
    RenderContextShadedWireFrame rcWireShadedFrame;
    RenderContextShaded rcShaded;
    RenderContextWireFrame rcWireFrame;
    RenderContextGame* rcGameView;
    RenderContextDisplay* renderContextDisplay;
    std::vector<float> memoryUsage;
    std::vector<float> fpsCounter;
    std::vector<MiniMLDisplay*> MLDisplays;
    bool openMachineState;
    bool openSceneEditor;
    bool openObjectView;
    bool openSceneViewOption;
    bool openMiniMLSettings;
    bool openRegression;
    int selectedObjects;
    int selectedSceneView;
    int nbHidden;
    int nbOutput;
    int heightHidden;
    float* heatMapMiniML;
    float learningRate;
    bool Plot;
    bool openMeshEditor = false;
    Network* network;

    public :

    DisplayerManager(){
        selectedSceneView = -1;
        memoryUsage = std::vector<float>(500);
        fpsCounter = std::vector<float>(25);
        openMachineState = true;
        openObjectView = true;
        openSceneEditor = true;
        openSceneViewOption = true;
        heatMapMiniML = new float[100*100];
        rcGameView = new RenderContextGame();
        network = nullptr;
    }

    DisplayerManager(DisplayerManager& copy){
        operator=(copy);
    }

    DisplayerManager operator=(DisplayerManager& copy){
        selectedSceneView = copy.selectedObjects;
        memoryUsage = copy.memoryUsage;
        fpsCounter = copy.fpsCounter;
        openMachineState = copy.openMachineState;
        openObjectView = copy.openMachineState;
        openSceneEditor = copy.openSceneEditor;
        openSceneViewOption = copy.openSceneEditor;
        rcGameView = copy.rcGameView; 
        this->network = copy.network;
        this->MLDisplays = copy.MLDisplays;
        return *this;
    }
    
    ~DisplayerManager(){
        for (size_t i = 0; i < RenderContextDisplays.size(); i++) {
            delete RenderContextDisplays[i];
        }
         for (size_t i = 0; i < MLDisplays.size(); i++) {
            delete MLDisplays[i];
        }
    }

    void ObjectEditor(Projet* projet);
    void AddRenderContextDisplay(RenderContextDisplay* renderWindow);
    void SceneViewParameter();
    void MachineState(Projet* projet);
    void SceneEditor(Projet* projet);
    void RenderAppOptions(Projet* projet);
    void RenderAllRenderWindows(int width,int height,Projet* projet);
    
    void RenderSceneViewOption();
    void MiniMLWindows();
    void DisplayerNetworkParameter();
    void RenderGameView(GameView* GameView,Projet* projet);

};