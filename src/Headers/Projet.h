#pragma once
#include "Scene.h"
#include "Mesh.h"
#include "ApplicationState.h"
#include "Object.h"
#include "Component.h"


class Material;

class Projet{
    Scene* scene;
    std::vector<Object*> objects;
    ApplicationState* appState;
    std::vector<Mesh*> defaultPrimitive;
    std::vector<Component*> Components;
    std::vector<GLuint> texID;
    std::vector<Material*> Materials;

    public :
    Projet(){
        // SetUpDefaultPrimitive();
        // SetUpDefaultObject();
        // SetUpComponents();
    }
    
    void AddScene(Scene* scene){
       this->scene = scene;
    }
    
    ApplicationState* getAppState(){
        return this->appState;
    }

    void SetAppState(ApplicationState* appState){
        this->appState = appState;
    }

    std::vector<GLuint> getTexID(){
        return  texID;
    }

    void AddTexID(GLuint id){
        texID.push_back(id);
    }

    Object* GetCube(){
        return objects[1];
    }

    Object* GetDragon(){
        return objects[2];
    }

    Object* GetLight(){
        return objects[3];
    }

    Object* GetCamera(){
        return objects[4];
    }

    Object* GetLineRenderer(){
        return objects[5];
    }

    Object* GetEmpty(){
        return objects[0];
    }

    Scene* GetScene(){
        return scene;
    }

    std::vector<Object*> GetObjs(){
        return objects;
    }

    Mesh* GetMesh(int i){
        return defaultPrimitive[i];
    }

    void deletMesh(int i){
        Mesh* mesh = GetMesh(i);
        defaultPrimitive.erase(defaultPrimitive.begin()+i);
        delete mesh;
    }

    std::vector<Mesh*> GetMeshPrimive(){
        return defaultPrimitive;
    }

    std::vector<Mesh*> GetPrimitive(){
        return this->defaultPrimitive;
    }

    std::vector<Material*> GetMaterials(){
        return this->Materials;
    }

    void AddPrimitive(Mesh* mesh){
        defaultPrimitive.push_back(mesh);
    }

    std::vector<Component*> GetComponents(){
        return Components;
    }

    void AddMaterial(Material* mat){
        this->Materials.push_back(mat);
    }


    void SetUpDefaultPrimitive();
    void SetUpDefaultObject();
    void SetUpComponents();


    void AddObjs(Object* mesh){
        objects.push_back(mesh);
    }

    void SetScene(Scene* scene){
        this->scene = scene;
    }
};

