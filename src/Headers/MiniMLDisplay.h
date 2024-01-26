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
    int nbOutput;
    int nbInput;
    float* heatMapMiniML;
    int nbHidden;
    int heightHidden;
    bool Plot;
    float learningRate;
    int sizex;
    int sizey;
    bool regression;
    int id;
    bool Trainning;
    int inputsize;
    std::string current;
    std::vector<float> data;
    float** input;
    float** output;
    bool updateHeat;
    GLuint texID;

    public : 
    bool open;
    int GetID(){
        return id;
    }
    MiniMLDisplay(bool regression,int id){
        sizex= 100;
        sizey= 100;
        Plot = true;
        nbHidden = 0;
        nbOutput = 1;
        learningRate = 0.1f;
        heightHidden = 0;
        Trainning = false;
        open = true;
        network = nullptr;
        heatMapMiniML = new float[sizex*sizey];
        this->regression = regression;
        inputsize = 0;
        input  = nullptr;
        output = nullptr;
        this->id = id;
        texID = id + 99;
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
        this->open = copy.open;
        this->heightHidden = copy.heightHidden;
        this->nbOutput = copy.nbOutput;
        this->nbHidden = copy.nbHidden;
        this->network = copy.network;
        this->Plot = copy.Plot;
        return (*this);
    }

    ~MiniMLDisplay(){
        delete heatMapMiniML;
        glDeleteBuffers(1,&texID);
    }

    void DisplayerNetworkParameter(){

        if(ImGui::Button("Create Network") && nbInput > 0){
            network = (Network*)MiniML::SetUpNetwork(nbInput,nbHidden,heightHidden,nbOutput,regression);
            heatMapMiniML = UpdateHeatMap(network,sizex,sizey,heatMapMiniML);
            updateHeat = true;
        }

        ImGui::SameLine();
        if(ImGui::Button("Train")){
            Trainning = !Trainning;
            heatMapMiniML = UpdateHeatMap(network,sizex,sizey,heatMapMiniML);
        }

        ImGui::SameLine();
        if(ImGui::Button("one Train")){
            MiniML::BackPropagation(network,input,inputsize,output,inputsize,0.001f,100000);
            heatMapMiniML = UpdateHeatMap(network,sizex,sizey,heatMapMiniML);
            updateHeat = true;
        }
        

        ImGui::SameLine();
        ImGui::PushItemWidth(150);
        ImGui::InputInt("Hidden",&nbHidden);
        ImGui::SameLine();
        ImGui::InputInt("Height Hidden",&heightHidden);
        ImGui::PopItemWidth();
        ImGui::Checkbox("Plot",&Plot);
        ImGui::SameLine();
        ImGui::InputFloat("LearningRate",&learningRate);
    }

    void DisplayerError(Network* network){
        if (ImPlot::BeginPlot("Error",ImVec2(1000,500))) {
                ImPlot::SetupAxesLimits(0,100,-.25f,1);
                float* error = MiniML::GetError(network);
                float* it = MiniML::GetIter(network);
                int n = MiniML::GetSizeError(network);
                float t= it[n-1];
                ImPlot::SetupAxisLimits(ImAxis_X1,0, t, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                ImPlot::PlotShaded("Error",it,error,n, -INFINITY);
                ImPlot::PopStyleVar();
                ImPlot::PlotLine("Error", it,error,n);
                ImPlot::EndPlot();
            }
    }

    float* UpdateHeatMap(Network* network,int sizex,int sizey,float* v){
        int size = MiniML::GetLayerSize(network,MiniML::GetNetworkSize(network)-1);
        std::vector<float> value;
        for (int j = 0; j < sizey; j++)
            for (int i = 0; i < sizex; i++)
            {
                float x = ((float)i)/sizex;
                float y = ((float)j)/sizey;
                for (int k = 0; k < 3; k++)
                {
                    float values[] = {x, 1.0f - y};
                    value.push_back(MiniML::SimulateNetwork(network,&values[0],2)[k%size]);
                }

            }

        glBindTexture(GL_TEXTURE_2D,this->texID);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,sizex,sizey,0,GL_RGB,GL_FLOAT,&value[0]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D,0);

        return v;
    }

    void DisplayNetwork(Network* network){
        if (network != nullptr && ImPlot::BeginPlot("##MarkerStyles", ImVec2(500,500)) ) {

                ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
                int height =((MiniML::GetLayerSize(network,0)+1)*10.0f)/2.0f;
                ImPlot::SetupAxesLimits(-10.0f, MiniML::GetNetworkSize(network)*10.0f, -height,height+10.0f );

                int layer = 3;
                int p = 3;
                for (int i = 0; i < MiniML::GetNetworkSize(network); i++)
                {
                    int size = MiniML::GetLayerSize(network,i);
                    for (int j = 0; j < size; j++)
                    {
                        ImS8 xs[2] = {(i)*(10),((i)+1)*(10)};
                        ImS8 ys[2] = {(size-j)*(10) - (size*10.0)/2.0f,(size-j)*(10)-(size*10.0)/2.0f};

                        ImGui::PushID(i*j);
                        ImPlot::SetNextMarkerStyle(1, 30.0, ImVec4(.3,0.3,0.7,1.0), 0.5f);
                        ImPlot::PlotLine("##Filled",xs, ys, 1);
                        ImGui::PopID();

                        for (int k = 0; k < MiniML::GetLayerRealSize(network,i+1); k++)
                        {
                            ys[1] = (MiniML::GetLayerSize(network,i+1)-k)*(10) - (MiniML::GetLayerSize(network,i+1)*10.0)/2.0f;
                            ImPlot::SetNextLineStyle( ImVec4(1.0,0.0,0.0,1.0));
                            ImPlot::PlotLine("##Filled",xs, ys, 2);
                            glm::vec2 v(xs[1] - xs[0],ys[1] - ys[0]);
                            v = glm::normalize(v);
                            std::string w = std::to_string(MiniML::GetWeight(network,i+1,k,j));
                            ImPlot::PlotText(w.c_str(), xs[0] +v.x*2.0f, ys[0]+v.y*2.0f);
                        }

                        std::string s;
                        if(i > 0 && i < MiniML::GetNetworkSize(network)-1){
                            s = "H";
                        }
                        else if(i==0){
                            s = "I";
                        }
                        else{
                            s = "O";
                        }
                        s = s+std::to_string(j);

                        if(j > MiniML::GetLayerSize(network,i)-1){
                            s = "B";
                        }

                        ImPlot::PlotText(s.c_str(), xs[0], ys[0]);
                    }
                    
                }
                
                
                ImPlot::EndPlot();
            }
    }

    void SetUpTestCaseRegression(){
        if (ImGui::BeginCombo("Input/ouput", current.c_str()))
        {
            data.clear();
            Trainning = false;
            if (ImGui::Selectable("Linear Simple", current == "Linear Simple")){
                current = "Linear Simple";
                nbOutput = 1;
                nbInput = 1;
                float r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
            }

            if (ImGui::Selectable("None Linear Simple", current == "None Linear Simple")){
                current = "None Linear Simple";
                 nbOutput = 1;
                nbInput = 1;
                float r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
                r = (((double) rand() / (RAND_MAX)));
                data.insert(data.end(),{r});
            }

            if(data.size() > 0){
                if(input != nullptr) delete input;
                if(output != nullptr) delete output;
                int lineSize = nbOutput+2;
                inputsize = data.size()/lineSize;
                input = new float*[inputsize];
                output = new float*[inputsize];
                for (int i = 0; i < inputsize; i++)
                {
                    input[i] = new float[2];
                    output[i] = new float[nbOutput];
                    for (int j = 0; j < lineSize; j++)
                    {
                        if(j < lineSize - nbOutput) input[i][j] = data[i*lineSize + j];
                        else output[i][j-(lineSize - nbOutput)] = data[i*lineSize + j];
                    }
                }
            }
        
            ImGui::EndCombo();
        }
    }

    void SetUpTestCaseClassification(){
        if (ImGui::BeginCombo("Input/ouput", current.c_str()) )
        {
            data.clear();
            Trainning = false;
            if (ImGui::Selectable("Xor", current == "Xor")){
                current = "Xor";
                nbInput = 2;
                nbOutput = 1;
                data.insert(data.end(),{0,1,1});
                data.insert(data.end(),{1,1,0});
                data.insert(data.end(),{1,0,1});
                data.insert(data.end(),{0,0,0});
            }

            if (ImGui::Selectable("Linear Simple", current == "Linear Simple")){
                current = "Linear Simple";
                nbInput = 2;
                nbOutput = 1;
                data.insert(data.end(),{.25f,.8f,0});
                data.insert(data.end(),{0.8f,.5f,1.0f});
                data.insert(data.end(),{.1,.6,0.0});
            }

            if (ImGui::Selectable("Cross", current == "Cross")){
                current = "Cross";
                nbInput = 2;
                nbOutput = 1;
                for (float i = 0.0; i <= 1.05f; i+=0.05f)
                {
                    for (float j = 0.0; j <= 1.05f; j+=0.05f)
                    {
                        data.insert(data.end(),{i,j});
                        if((i > 0.405 && i < 0.605) || (j > 0.405 && j < 0.605)){
                            data.insert(data.end(),{1});
                        }else{
                            data.insert(data.end(),{0});
                        }
                    }
                }
            }

            if (ImGui::Selectable("3 classes", current == "3 classes")){
                current = "3 classes";
                nbInput = 2;
                nbOutput = 3;
                int k = 30;
                while(k > 0){
                    float x = (((double) rand() / (RAND_MAX)))*0.45f;
                    float y = (((double) rand() / (RAND_MAX)))*0.50f;
                    data.insert(data.end(),{x,y});
                    data.insert(data.end(),{1,0,0});
                    k--;
                }
                k = 30;
                while(k > 0){
                    float x = (((double) rand() / (RAND_MAX)));
                    float y = (((double) rand() / (RAND_MAX)))*0.45f;
                    data.insert(data.end(),{x,1.0f-y});
                    data.insert(data.end(),{0,1,0});
                    k--;
                }

                k = 30;
                while(k > 0){
                    float x = (((double) rand() / (RAND_MAX)))*0.45f;
                    float y = (((double) rand() / (RAND_MAX)))*0.5f;
                    data.insert(data.end(),{0.55f+ x,y});
                    data.insert(data.end(),{0,0,1});
                    k--;
                }
            }

            if (ImGui::Selectable("3 cross", current == "3 cross")){
                current = "3 cross";
                nbInput = 2;
                nbOutput = 3;
                for (float i = 0.0; i <= 1.05f; i+=0.05f)
                {
                    for (float j = 0.0; j <= 1.05f; j+=0.05f)
                    {
                        data.insert(data.end(),{i,j});
                        if((cos((i)*4*glm::pi<float>() + glm::pi<float>()/2.0f ) > 0)){
                            if((sin((j)*4*glm::pi<float>() + glm::pi<float>()/2.0f ) > 0)){
                                data.insert(data.end(),{0,0,1});
                            }else{
                                data.insert(data.end(),{1,0,0});
                            }
                        }else{
                            if((sin((j)*4*glm::pi<float>() + glm::pi<float>()/2.0f ) > 0)){
                                data.insert(data.end(),{1,0,0});
                            }else{
                                data.insert(data.end(),{0,1,0});
                            }
                        }
                    }
                }
            }

            if(data.size() > 0){
                if(input != nullptr) delete input;
                if(output != nullptr) delete output;
                int lineSize = nbOutput+2;
                inputsize = data.size()/lineSize;
                input = new float*[inputsize];
                output = new float*[inputsize];
                for (int i = 0; i < inputsize; i++)
                {
                    input[i] = new float[2];
                    output[i] = new float[nbOutput];
                    for (int j = 0; j < lineSize; j++)
                    {
                        if(j < lineSize - nbOutput) input[i][j] = data[i*lineSize + j];
                        else output[i][j-(lineSize - nbOutput)] = data[i*lineSize + j];
                    }
                }
            }

            ImGui::EndCombo();
        }
    }

    void PlotClassification(){
        if(network != nullptr && updateHeat){
            ImPlot::PushColormap(ImPlotColormap_Jet);

            ImS8 x[2] = {0.0f,0.0f};
            ImS8 y[2] =  {0.0f,0.0f};

            if (Plot && ImPlot::BeginPlot("##Heatmap2",ImVec2(500,500)) && 
            inputsize > 0) {
                ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
                ImPlot::SetupAxesLimits(0,1,0,1);
                //ImPlot::PlotHeatmap("heatman",&heatMapMiniML[0],sizex,sizey,0.f,1.f,nullptr);
                static ImVec2 bmin(0,0);
                static ImVec2 bmax(1,1);
                static ImVec2 uv0(0,0);
                static ImVec2 uv1(1,1);
                ImPlot::PlotImage("heatmap",(void*)(intptr_t)this->texID, bmin, bmax, uv0,uv1);
                for (int i = 0; i < inputsize; i++)
                {
                    float x = input[i][0];
                    float y = input[i][1];

                    if(output[i][0] == 0 && (nbOutput > 1 && output[i][2] == 1)){
                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 10,ImVec4(0.0,0.0,1.0,1.0));
                    }
                    else if(output[i][0] == 1){
                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 10,ImVec4(1.0,0.0,0.0,1.0));
                    }
                    else if(output[i][1] == 1){
                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 10,ImVec4(0.0,1.0,0.0,1.0));
                    }
                    
                    ImPlot::PlotScatter("Input", &x, &y, 1);
                }
                

                ImPlot::EndPlot();
            }
            ImPlot::PopColormap();

        DisplayerError(network);
        }
        ImGui::End();
    }

    void PlotRegression(){
        if(network != nullptr && updateHeat){
            
            ImPlot::PushColormap(ImPlotColormap_Jet);

            ImS8 x[2] = {0.0f,0.0f};
            ImS8 y[2] =  {0.0f,0.0f};

            if (Plot){
                static float xs1[1001], ys1[1001];
                for (int i = 0; i < 1001; ++i) {
                    xs1[i] = i * 0.001f;
                    ys1[i] = MiniML::SimulateNetwork(network,&xs1[i],1)[0];
                }

                if (ImPlot::BeginPlot("Line Plots",ImVec2(500,500))) {
                    ImPlot::SetupAxisLimits(0,1,0,1);
                    ImPlot::SetNextLineStyle(ImVec4(1.0,0.0,0.0,1.0));
                    ImPlot::PlotLine("Network(x)", xs1, ys1, 1001);
                    for (int i = 0; i < inputsize; i++)
                    {
                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 10,ImVec4(0.0,1.0,1.0,1.0));
                        ImPlot::PlotScatter("Input", &input[i][0], &output[i][0], 1);
                    }
                    ImPlot::EndPlot();
                }
            }
            ImPlot::PopColormap();
           DisplayerError(network);
        }
        ImGui::End();
    }

    void RenderMiniML(){
        if(open){
            char buffer[32]; 
            char* str = "MLP Classification View## ";
            if(regression){
                str = "MLP Regression View## ";
            }
            sprintf(buffer, "%s%d", str,this->id);
            ImGui::Begin(buffer,&open);


            if(regression){
                SetUpTestCaseRegression();
            }
            else{
                SetUpTestCaseClassification();
            }

            DisplayerNetworkParameter();
            DisplayNetwork(network);

            if(Trainning && network != nullptr){
               
            }

            ImGui::SameLine();
            if(regression){
                PlotRegression();
                if(Trainning && network != nullptr){
                    MiniML::BackPropagation(network,input,inputsize,output,inputsize,learningRate,100000);
                    updateHeat = true;
                }

            }else{
                if(Trainning && network != nullptr){
                    MiniML::BackPropagation(network,input,inputsize,output,inputsize,learningRate,2000);
                    if(Plot && !regression){
                        heatMapMiniML = UpdateHeatMap(network,sizex,sizey,heatMapMiniML);
                        updateHeat = true;
                    }
                }
                PlotClassification();
            }
        }

    }
};