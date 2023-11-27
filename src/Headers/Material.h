#pragma once
// glew and glfw include
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Object.h"
#include "Shader.h"

class Material
{
    protected:
    Shader* shader;
    float* color;
    char* name;
    private:

    public:
    
    Material(Shader* _shader,char* _name):shader(_shader),name(_name){
        color = new float[3];
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
    }
    
    Material(Material& copy){
        this->shader = copy.shader;
        this->name = copy.name;
        color[0] = copy.color[0];
        color[1] = copy.color[1];
        color[2] = copy.color[2];
    }

    Material operator=(Material& copy){
        this->shader = copy.shader;
        this->name = copy.name;
        color[0] = copy.color[0];
        color[1] = copy.color[1];
        color[2] = copy.color[2];
        return *this;
    }

    ~Material(){
        delete color;
    }

    Shader* GetShader(){
        return shader;
    }

    virtual void ApplyParameter(){
        glUniform3fv(glGetUniformLocation(shader->GetIDX(), "color"), 1,&color[0]); 
    }

    virtual void Editor(){
        ImGui::Text("Material");
        ImGui::ColorPicker3("Color",color);
    }

    char* GetName(){
        return this->name;
    }


        
};

class PBRMaterial: public Material{
    float* Ambiante;
    float* DiffuseColor;
    float* Roughness;
    float* SpecularColor;
    public : 

    PBRMaterial(Shader* _shader,char* _name):Material(_shader,_name){
        Ambiante = new float[3];
        DiffuseColor = new float[3];
        Roughness = new float[3];
        SpecularColor = new float[3];
    }
    
    PBRMaterial(PBRMaterial& copy):Material(copy){
       for (int i = 0; i < 3; i++)
       {
        Ambiante[i] = copy.Ambiante[i];
        DiffuseColor[i] = copy.DiffuseColor[i];
        Roughness[i] = copy.Roughness[i];
        SpecularColor[i] = copy.SpecularColor[i];
       }
       
    }

    PBRMaterial operator=(PBRMaterial& copy){
        this->operator=(copy);
        for (int i = 0; i < 3; i++)
        {
            Ambiante[i] = copy.Ambiante[i];
            DiffuseColor[i] = copy.DiffuseColor[i];
            Roughness[i] = copy.Roughness[i];
            SpecularColor[i] = copy.SpecularColor[i];
        }

       return *this;
    }

    virtual void Editor(){
        ImGui::ColorPicker3("Ambiante Color",Ambiante);
        ImGui::ColorPicker3("DiffuseColor",DiffuseColor);
    }

    virtual void ApplyParameter(){
        glUniform3fv(glGetUniformLocation(shader->GetIDX(), "color"), 1,&color[0]); 
        glUniform3fv(glGetUniformLocation(shader->GetIDX(), "u_AmbianteColor"), 1,&Ambiante[0]); 
        glUniform3fv(glGetUniformLocation(shader->GetIDX(), "u_DiffuseColor"), 1,&DiffuseColor[0]); 
        return;
    }

    ~PBRMaterial(){
        delete Ambiante;
        delete DiffuseColor;
        delete Roughness;
        delete SpecularColor;
    }
};


class WaterMaterial: public Material{
    float time;   
    float direction[2];
    float frequency;
    float amplitude;
    float steepness;
    float speed;

    public : 
    WaterMaterial(Shader* _shader,char* _name):Material(_shader,_name){
       time = 0;
       direction[0] = 1.0f;
       direction[1] = 0.0f;

       frequency = 1.0;
       amplitude = 1.0;
       steepness = .5;
       speed = 1.0;
    }
    
    WaterMaterial(WaterMaterial& copy):Material(copy){
       
    }

    WaterMaterial operator=(WaterMaterial& copy){
        this->operator=(copy);
        time = time;

       return *this;
    }

    virtual void Editor(){
        ImGui::DragFloat2("Direction",&direction[0]);
        ImGui::DragFloat("Frequency",&frequency,0.1f);
        ImGui::DragFloat("Amplitude",&amplitude,0.1f);
        ImGui::DragFloat("Steepness",&steepness,0.1f);
        ImGui::DragFloat("Speed",&speed);
    }

    virtual void ApplyParameter(){

        glUniform1f(glGetUniformLocation(shader->GetIDX(), "time"),	glfwGetTime()); 
        glUniform1fv(glGetUniformLocation(shader->GetIDX(), "wavesData.frequency"),1,	&frequency); 
        glUniform1fv(glGetUniformLocation(shader->GetIDX(), "wavesData.steepness"),	1,&steepness); 
        glUniform1fv(glGetUniformLocation(shader->GetIDX(), "wavesData.speed"),	1,&speed); 
        glUniform2fv(glGetUniformLocation(shader->GetIDX(), "wavesData.direction"),	1,&direction[0]); 
        glUniform1fv(glGetUniformLocation(shader->GetIDX(), "wavesData.amplitude"),	1,&amplitude); 


        // glUniform1f(glGetUniformLocation(shader->GetWireFrame(), "time"),	glfwGetTime()); 
        // glUniform1fv(glGetUniformLocation(shader->GetWireFrame(), "wavesData.amplitude"),1,&amplitude); 
        // glUniform1fv(glGetUniformLocation(shader->GetWireFrame(), "wavesData.steepness"),1,	&steepness); 
        // glUniform1fv(glGetUniformLocation(shader->GetWireFrame(), "wavesData.frequency"),1,	&frequency); 
        // glUniform1fv(glGetUniformLocation(shader->GetWireFrame(), "wavesData.speed"),1,	&speed); 

        
        return;
    }

    ~WaterMaterial(){
    }
};
