#pragma once
// glew and glfw include
#include "GL/glew.h"
#include "GLFW/glfw3.h"

// imgui include
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "implot.h"
#include "MiniML.hpp"
#include <vector>
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <string>

class Network;
class MiniMLDisplay{
    Network* network;
    float** input;
    float* heatMapMiniML;
    float** output;
    int nbOutput,nbInput,nbHidden,heightHidden,inputsize;
    int sizex,sizey;
    int id;
    bool regression,linear,Trainning,Plot,updateHeat,open;
    float learningRate;
    std::string current;
    std::vector<float> data;
    GLuint texID;

    public : 
    MiniMLDisplay(bool regression,bool linear,int id){
        sizex= 100;
        sizey= 100;
        Plot = true;
        open = true;
        nbHidden = 0;
        nbOutput = 1;
        learningRate = 0.1f;
        heightHidden = 0;
        Trainning = false;
        network = nullptr;
        heatMapMiniML = new float[sizex*sizey];
        this->regression = regression;
        inputsize = 0;
        input  = nullptr;
        output = nullptr;
        this->id = id;
        this->linear = linear;
        glGenTextures(1,&texID);
        glBindTexture(GL_TEXTURE_2D,texID);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,sizex,sizey,0,GL_RGB,GL_UNSIGNED_BYTE,(void*)0);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D,0);
    }

    MiniMLDisplay(MiniMLDisplay& copy){
        operator=(copy);
    }

    MiniMLDisplay operator=(MiniMLDisplay& copy){
        this->heatMapMiniML = copy.heatMapMiniML;
        this->sizex = copy.sizex;
        this->sizey = copy.sizey;
        this->learningRate = copy.learningRate;
        this->heightHidden = copy.heightHidden;
        this->nbOutput = copy.nbOutput;
        this->nbHidden = copy.nbHidden;
        this->network = copy.network;
        this->Plot = copy.Plot;
        this->regression = copy.regression;
        this->linear = copy.linear;
        return (*this);
    }

    ~MiniMLDisplay(){
        delete heatMapMiniML;
        glDeleteTextures(1,&texID);
        if(input != nullptr){
            delete input;
        }

        if(output != nullptr){
            delete output;
        }
    }
    int GetID(){  return id;}
    bool GetOpen(){ return open;}

    float* UpdateHeatMap(Network* network,int sizex,int sizey,float* v);
    void DisplayerError(Network* network);
    void DisplayNetwork(Network* network);
    void SetUpTestCaseClassification();
    void DisplayerNetworkParameter();
    void SetUpTestCaseRegression();
    void PlotClassification();
    void PlotRegression();
    void RenderMiniML();
};