#pragma once
#include "Mesh.h"

#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>
#include <string>  
#include <vector>

// imgui include
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Component;
class Scene;
class Projet;

enum ObjectType{
    o_ClassicObject,
    o_Light,
    o_Camera,
    o_Loader,
    o_LineRenderer
};

class Object{

    protected :
    Mesh* mesh;
    Projet* projet;
    glm::mat4 transformation;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    uint32_t programShader;
    std::string name;
    ObjectType objectType;
    std::vector<Component*> components;
    ImVec4 color;

    public:

    Object() = default;

    Object(Mesh* _mesh,std::string _name,ObjectType _objectType):mesh(_mesh),name(_name){
        transformation = glm::mat4(1.0f);
        rotation = glm::vec3({0.0f,0.0f,0.0f});
        position = glm::vec3({0.0f,0.0f,0.0f});
        scale = glm::vec3({1.0f,1.0f,1.0f});
        objectType = _objectType;
        color = ImVec4(0.85f,0.85f,0.85f,1.0f);
        AddComponents();
    }

    Object(const Object& copy){
        this->mesh = copy.mesh;
        this->name = copy.name;
        this->objectType = copy.objectType;
        rotation = glm::vec3(copy.rotation);
        position = glm::vec3(copy.position);
        scale = glm::vec3(copy.scale);
        this->color = copy.color;
        this->programShader = copy.programShader;

        CopyComponents(copy.components);
    }

    Object operator=(const Object& copy){
        this->mesh = copy.mesh;
        this->name = copy.name;
        this->objectType = copy.objectType;
        this->programShader = copy.programShader;
        this->color = copy.color;
        CopyComponents(copy.components);
    }
    
    ~Object(){
        DeleteComponents();
    }

    void DeleteComponent(int i);
    void SetUp();

    void DeleteComponents();
    void AddComponents();
    void AddComponent(Component* comp){ components.push_back(comp); }
    void CopyComponents(std::vector<Component*> copy);

    char* GetName(){ return (char*)name.c_str(); }
    Mesh* GetMesh()const{ return mesh; };
    ImVec4 GetColor(){ return this->color; }
    Projet* GetProjet(){ return projet; }
    glm::vec3 GetPosition(){ return position; }
    glm::vec3 GetRotation(){ return rotation; }
    glm::vec3 GetScale(){ return scale; }
    glm::vec3 GetForward(){ return glm::vec3( GetTransformation() * glm::vec4({0.0,0.0,1.0,0.0})); }
    glm::mat4 GetTransformation();
    ObjectType GetObjectType(){ return objectType;}
    std::string GetStrName(){ return this->name; }
    std::vector<Component*> GetComponents(){ return components; }
    
    void SetName(std::string str){ name = str; }
    void SetPosition(glm::vec3 position){ this->position = position; }
    void SetRotation(glm::vec3 rotation){ this->rotation = rotation;}
    void SetObjectType(ObjectType type);
    void SetScale(glm::vec3 scale){ this->scale = scale; }
    void SetColor(ImVec4 color){ this->color = color; }
    void SetProjet(Projet* p){ projet = p; }

};
