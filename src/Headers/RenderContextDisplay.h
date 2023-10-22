#pragma once
#include<vector>

// imgui include
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "RenderContext.h"

class Scene;

class RenderContextDisplay{

    bool p_open;
    int idx_renderContext;
    std::vector<RenderContext*> renderContextes;
    char* label;
    glm::mat4* MVP;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 padding;
    RenderType renderType;
    char* item_current;
    float OrthographiqueSize;
    float perspectiveFov;
    float nearClip;
    float farClip;

    public :
    RenderContextDisplay(){
        OrthographiqueSize = 5.0f;
        position = glm::vec3({ 0.f, .6f,5.0f });
        rotation = glm::vec3(0.0f);
        padding = glm::vec3(0.0f);
        idx_renderContext= 0;
        p_open = true;
        MVP = new glm::mat4[2];
        MVP[0] =  glm::translate(glm::mat4(1.f), -position);
        MVP[0] *= glm::rotate(glm::mat4(1.0f), 15 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
        MVP[0] *= glm::rotate(glm::mat4(1.0f), 15 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
        MVP[1] = glm::mat4(1.0f);
        renderType = Perspective;
        perspectiveFov = 45.0f;
        nearClip = 0.01f;
        farClip = 1000.0f;
    }

    

    RenderContextDisplay(const RenderContextDisplay& copy){
        this->p_open = copy.p_open;
        this->idx_renderContext = copy.idx_renderContext;
        this->renderContextes = std::vector<RenderContext*>(copy.renderContextes);

        MVP = new glm::mat4[2];
        MVP[0] = copy.MVP[0];
        MVP[1] =  copy.MVP[1];
    }

    void SetRenderType(RenderType type){
        renderType = type;
    }

    float GetNearClip(){
        return this->nearClip;
    }

    void SetNearClip(float n){
        this->nearClip = n;
    }

    float GetFarClip(){
        return this->farClip;
    }

    void SetFarClip(float n){
        this->farClip = n;
    }

    RenderType GetRenderType(){
        return this->renderType;
    }

    void SetOrthoSize(float s){
        this->OrthographiqueSize = s;
    }

    float GetOrthoSize(){
        return this->OrthographiqueSize;
    }

    
    void SetFov(float s){
        this->perspectiveFov = s;
    }

    float GetFov(){
        return this->perspectiveFov;
    }

    glm::mat4* GetTransform(){

        
        MVP[0] =  glm::translate(glm::mat4(1.f), -position);
        MVP[0] *= glm::rotate(glm::mat4(1.0f), rotation.x * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        MVP[0] *= glm::rotate(glm::mat4(1.0f), rotation.y * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        MVP[0] *= glm::rotate(glm::mat4(1.0f), rotation.z * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        MVP[0] *= glm::translate(glm::mat4(1.f), -padding);

        return MVP;
    }

    std::vector<RenderContext*> getRenderContextes(){
        return this->renderContextes;
    }

    glm::vec3 GetRight(){
        glm::vec4 right = glm::vec4({1.0f,0.0f,0.0f,0.0f});
 
        return glm::vec3(right*MVP[0]);
    }

    glm::vec3 GetForward(){
        glm::vec4 forward = glm::vec4({0.0f,0.0f,1.0f,0.0f});
 
        return glm::vec3(forward*MVP[0]);
    }

    glm::vec3 GetPosition(){
        return position;
    }

    glm::vec3 GetPadding(){
        return padding;
    }

    glm::vec3 GetRotation(){
        return rotation;
    }

    void SetPosition(glm::vec3 pos){
        position = pos;
    }

     void SetPadding(glm::vec3 pad){
        padding = pad;
    }

    void SetRotation(glm::vec3 rot){
       rotation = rot;
    }

    RenderContextDisplay operator=(const RenderContextDisplay& copy){
        this->p_open = copy.p_open;
        this->idx_renderContext = copy.idx_renderContext;
        this->renderContextes = std::vector<RenderContext*>(copy.renderContextes);

        MVP = new glm::mat4[2];
        MVP[0] = copy.MVP[0];
        MVP[1] =  copy.MVP[1];
        return *this;
    }

    ~RenderContextDisplay(){
        for(auto p : renderContextes){
            delete p;
        }

        delete[] MVP;
    }

    bool GetOpen(){
        return this->p_open;
    } 

    void AddRender(RenderContext* render){
        renderContextes.push_back(render);
        item_current = renderContextes[0]->GetLabel();
    }

    bool DisplayRenderWindow(int width,int height,Scene* scene,char* label);
};