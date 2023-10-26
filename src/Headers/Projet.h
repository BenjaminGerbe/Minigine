#pragma once
#include "Scene.h"
#include "Mesh.h"
#include "ApplicationState.h"
#include "Object.h"


class Projet{
    Scene* scene;
    std::vector<Object*> objects;
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

    std::vector<Object*> GetObjs(){
        return objects;
    }

    void AddObjs(Object* mesh){
        objects.push_back(mesh);
    }

    void SetScene(Scene* scene){
        this->scene = scene;
    }
};

