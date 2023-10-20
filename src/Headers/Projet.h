#pragma once
#include "Scene.h"
#include "Mesh.h"

class Projet{
    Scene* scene;
    std::vector<Mesh*> meshes;

    public :
    Projet()=default;

    void AddScene(Scene* scene){
       this->scene = scene;
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

