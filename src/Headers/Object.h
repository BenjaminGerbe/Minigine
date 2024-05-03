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
#include <new>

// imgui include
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
class Component;
class Scene;
class Projet;
class Material;

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
    Material* material;
    std::string name;
    ObjectType objectType;
    std::vector<Component*> components;
    ImVec4 color;
    GLuint TexID;

    public:
    static Object* pool;
    static bool* contain;
    static int currentIdx;
    static int countObjectMax;

    Object() = default;

    Object(Mesh* _mesh,Material* _mat,std::string _name,ObjectType _objectType):mesh(_mesh),name(_name),material(_mat){
        transformation = glm::mat4(1.0f);
        rotation = glm::vec3({0.0f,0.0f,0.0f});
        position = glm::vec3({0.0f,0.0f,0.0f});
        scale = glm::vec3({1.0f,1.0f,1.0f});
        objectType = _objectType;
        color = ImVec4(0.85f,0.85f,0.85f,1.0f);
        TexID = 1;
        AddComponents();
        std::cout << " call Constructor " << std::endl;
    }

    Object(const Object& copy){
        this->mesh = copy.mesh;
        this->name = copy.name;
        this->objectType = copy.objectType;
        rotation = glm::vec3(copy.rotation);
        position = glm::vec3(copy.position);
        scale = glm::vec3(copy.scale);
        this->color = copy.color;
        this->material = copy.material;
        //this->programShader = copy.programShader;
        this->TexID = copy.TexID;
        CopyComponents(copy.components);
    }

    void* operator new(size_t size){   
        int idx = currentIdx;
        if(contain[idx] == false){
            for (size_t i = currentIdx; i < countObjectMax+currentIdx; i++)
            {
                int idx = i%countObjectMax;
                if(contain[idx] == true){
                    currentIdx = idx;
                    break;
                }
            }
            
        }
        contain[currentIdx] = false;
        &pool[currentIdx];
        int tmp = currentIdx;
        currentIdx++;
        currentIdx = (currentIdx)%countObjectMax;
        return &pool[tmp];
    }

    void operator delete(void* pointer){
        auto idx = ((Object*)(pointer)- (Object*)&pool[0]);
        contain[idx] = true;
        pool[idx].~Object();
    }

    static void CreatePool(int count){
        pool = new Object[count];
        contain = new bool[count];
        countObjectMax = count;
    };

    Object operator=(const Object& copy){
        return Object(copy);
    }
    
    ~Object(){ DeleteComponents(); }

    void DeleteComponent(int i);
    void SetUp();

    void DeleteComponents();
    void AddComponents();
    void AddComponent(Component* comp){ components.push_back(comp); }
    void CopyComponents(std::vector<Component*> copy);

    char* GetName(){ return (char*)name.c_str(); }
    Mesh* GetMesh()const{ return mesh; };
    ImVec4 GetColor(){ return this->color; }
    GLuint GetTexId(){ return this->TexID;};
    Projet* GetProjet(){ return projet; }
    glm::vec3 GetPosition(){ return position; }
    glm::vec3 GetRotation(){ return rotation; }
    glm::vec3 GetScale(){ return scale; }
    glm::vec3 GetForward(){ return glm::vec3( GetTransformation() * glm::vec4({0.0,0.0,1.0,0.0})); }
    glm::vec3 GetUp(){ return glm::vec3( GetTransformation() * glm::vec4({0.0,1.0,0.0,0.0})); }
    glm::mat4 GetTransformation();
    ObjectType GetObjectType(){ return objectType;}
    std::string GetStrName(){ return this->name; }
    std::vector<Component*> GetComponents(){ return components; }
    Material* GetMaterial(){return this->material;};

    void SetMaterial(Material* mat){this->material = mat;};
    void SetMesh(Mesh* mesh){ this->mesh = mesh;};
    void SetName(std::string str){ name = str; }
    void SetPosition(glm::vec3 position){ this->position = position; }
    void SetRotation(glm::vec3 rotation){ this->rotation = rotation;}
    void SetObjectType(ObjectType type);
    void SetScale(glm::vec3 scale){ this->scale = scale; }
    void SetColor(ImVec4 color){ this->color = color; }
    void SetProjet(Projet* p){ projet = p; }
    void SetTexID(GLuint id){ this->TexID = id; };

};
