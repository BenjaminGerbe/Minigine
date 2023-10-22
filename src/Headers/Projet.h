#pragma once
#include "Scene.h"
#include "Mesh.h"
#include "ApplicationState.h"

class Projet{
    Scene* scene;
    std::vector<Mesh*> meshes;
    ApplicationState* appState;

    public :
    Projet()=default;

    void AddScene(Scene* scene){
       this->scene = scene;
    }
    
    ApplicationState* getAppState(){
        return this->appState;
    }

    void SetAppState(ApplicationState* appState){
        this->appState = appState;
    }

    Scene* GetScene(){
        return scene;
    }

    std::vector<Mesh*> GetMeshes(){
        return meshes;
    }

    void AddMesh(Mesh* mesh){
        meshes.push_back(mesh);
    }

    void SetScene(Scene* scene){
        this->scene = scene;
    }
};

