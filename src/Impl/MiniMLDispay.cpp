#include "../Headers/MiniMLDisplay.h"
#include <fstream>
#include "../Headers/json.hpp"
#include <iostream>

float* MiniMLDisplay::UpdateHeatMap(Network* network,int sizex,int sizey,float* v){
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


void MiniMLDisplay::DisplayerNetworkParameter(){

    if(ImGui::Button("Create Network") && nbInput > 0){
        if(this->type == NetworkType::RBF){
            network = (Network*)MiniML::SetUpNetwork(data.size()/(nbInput+nbOutput),nbHidden,heightHidden,nbOutput,regression);
        }
        else{
            network = (Network*)MiniML::SetUpNetwork(nbInput,nbHidden,heightHidden,nbOutput,regression);
        }
        if(Plot && !regression){
            heatMapMiniML = UpdateHeatMap(network,sizex,sizey,heatMapMiniML);
        }
        updateHeat = true;
    }

    ImGui::SameLine();
    if(ImGui::Button("Train")){
        Trainning = !Trainning;
        if(Plot && !regression){
            heatMapMiniML = UpdateHeatMap(network,sizex,sizey,heatMapMiniML);
        }
    }

    ImGui::SameLine();
    if(ImGui::Button("one Train")){
        if(type == NetworkType::MLP){
            MiniML::BackPropagation(network,input,inputsize,output,inputsize,learningRate,interationMax);
        }
        else if(type == NetworkType::Linear){
            MiniML::LinearPropagation(network,input,inputsize,output,inputsize,learningRate,interationMax);
        }
        else if(type == NetworkType::RBF){
            MiniML::RBFPropagation(network,input,inputsize,nbInput,output,inputsize,learningRate,this->kvalue, interationMax);
        }

        if(Plot && !regression){
            heatMapMiniML = UpdateHeatMap(network,sizex,sizey,heatMapMiniML);
        }

        updateHeat = true;
    }

    ImGui::SameLine();
    if(ImGui::Button("Save Weigts")){
        MiniML::SaveWeights(network,"weight_save");
    }
    
    ImGui::SameLine();
    if(ImGui::Button("Load Weigts")){
        MiniML::LoadWeights(network,"weight_save");
    }

    ImGui::PushItemWidth(150);
    if(type == NetworkType::MLP){
        ImGui::InputInt("Hidden",&nbHidden);
        nbHidden = std::max<int>(0,nbHidden);
        ImGui::SameLine();
        ImGui::InputInt("Height Hidden",&heightHidden);
        heightHidden = std::max<int>(0,heightHidden);
    }
  
    ImGui::Checkbox("Plot",&Plot);
    ImGui::SameLine();
    ImGui::Checkbox("NetworkVisualiseur",&NetworkVisualiseur);
    ImGui::SameLine();
    if(this->type != NetworkType::RBF){
        ImGui::InputDouble("LearningRate",&learningRate);
    }else{
        ImGui::InputDouble("influenceZone",&learningRate);
        ImGui::SameLine();
        ImGui::InputInt("Kvalue",&this->kvalue);
    }
    ImGui::SameLine();
    ImGui::InputInt("IterationMax",&interationMax);
    ImGui::PopItemWidth();
}

void MiniMLDisplay::DisplayerError(Network* network){
    
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
                static int counter = 0;
                static float lastValue;
                if(storedBoards.size() > 0){
                    if(Trainning){
                        counter--;
                        float error =0;
                        if(counter <= 0){
                            counter = 100;
                            auto start = std::chrono::system_clock::now();
                            float error =0;
                            #pragma omp parallel for reduction(+:error)
                            for (int i = 0; i < inputsizeboard; i++)
                            {
                                error+= std::abs(outputBoards[i][0] - MiniML::SimulateNetwork(network,inputBoards[i],nbInput)[0]);
                            }
                            lastValue = error/(float)inputsizeboard;
                            errorBoards.push_back(error/(float)inputsizeboard);
                            auto end = std::chrono::system_clock::now();

                            std::chrono::duration<double> elapsed_seconds = end-start;
                            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
                        
                            std::cout << "finished computation at " << std::ctime(&end_time)
                                    << "elapsed time: " << elapsed_seconds.count() << "s"
                                    << std::endl;
                        }
                        else{
                            errorBoards.push_back(lastValue);
                        }
                    }
                  
                    ImPlot::PlotLine("Error2", it,&errorBoards[0],n);
                }
            ImPlot::EndPlot();
        }
    }


 void MiniMLDisplay::DisplayNetwork(Network* network){
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
                    double xs[2] = {(i)*(10),((i)+1)*(10)};
                    double ys[2] = {(size-j)*(10) - (size*10.0)/2.0f,(size-j)*(10)-(size*10.0)/2.0f};

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

 void MiniMLDisplay::SetUpTestCaseRegression(){
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
           
            }
            
            // Dataset needs to be 10k to use gamesStored correctly
            if (ImGui::Selectable("ReadJsonGame", current == "ReadJsonGame"))
            {
                current = "ReadJsonGame";
                nbInput = 768;
                nbOutput = 1;
                // Amount of games to keep stored and not added to our data
                int gamesStored = 10000;
                std::ifstream file("games.json");
                nlohmann::json j;
                file >> j;

                if(input != nullptr){
                    delete input;
                }
                if(output != nullptr){
                    delete output;
                }

                int counter = 0;
                int lineSize = nbOutput+nbInput;
                inputsize = (40000);
                int idx = 0;
                input = new float*[inputsize];
                output = new float*[inputsize];
                for (int i = 0; i < 40000; i++)
                {
                    input[i] = new float[nbInput];
                    output[i] = new float[nbOutput];
            
                }
                

                for (auto& element : j) {
                    std::vector<int> board_state = element["board_state"];
                    std::string evaluation = element["evaluation"];
                    int evaluation_int = 0;

                    // Checkmate handling
                    if (evaluation[0] == '#') {
                        char sign = evaluation[1];

                        if (sign == '+') {
                            evaluation_int = 2000; // Positive checkmate
                        }
                        else if (sign == '-') {
                            evaluation_int = -2000; // Negative checkmate
                        }
                        else {
                            std::cerr << "Invalid sign after #." << std::endl;

                        }
                    }
                    else {
                        evaluation_int = std::stoi(evaluation);
                    }

                    counter++;
                    evaluation_int = std::clamp(evaluation_int,-2000,2000);
                    if (counter <= 50000 - gamesStored) {
                    
                        for (int k = 0; k < nbInput; k++)
                        {
                            input[idx][k] = board_state[k];
                        }
                        output[idx][0] = (evaluation_int);
                        idx++;
                    }
                    else {
                        storedBoards.insert(storedBoards.end(), board_state.begin(), board_state.end());
                        storedBoards.push_back(evaluation_int);
                    }

                }
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
                int lineSize = nbOutput+nbInput;
                inputsize = data.size()/lineSize;
                input = new float*[inputsize];
                output = new float*[inputsize];
                for (int i = 0; i < inputsize; i++)
                {
                    input[i] = new float[nbInput];
                    output[i] = new float[nbOutput];
                    for (int j = 0; j < lineSize; j++)
                    {
                        if(j < lineSize - nbOutput) input[i][j] = data[i*lineSize + j];
                        else output[i][j-(nbInput)] = data[i*lineSize + j];
                    }
                }
            }
        
            ImGui::EndCombo();
        }
    }

void MiniMLDisplay::SetUpTestCaseClassification(){
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

        // Dataset needs to be 10k to use gamesStored correctly
        if (ImGui::Selectable("ReadJsonGame", current == "ReadJsonGame"))
        {
            current = "ReadJsonGame";
            nbInput = 768;
            nbOutput = 1;
            // Amount of games to keep stored and not added to our data
            int gamesStored = 10000;
            std::ifstream file("games.json");
            nlohmann::json j;
            file >> j;

            if(input != nullptr){
                delete input;
            }
            if(output != nullptr){
                delete output;
            }

            int counter = 0;
            int lineSize = nbOutput+nbInput;
            inputsize = (40000);
            int idx = 0;
            input = new float*[inputsize];
            output = new float*[inputsize];
            for (int i = 0; i < 40000; i++)
            {
                input[i] = new float[nbInput];
                output[i] = new float[nbOutput];
           
            }
            

            for (auto& element : j) {
                std::vector<int> board_state = element["board_state"];
                std::string evaluation = element["evaluation"];
                int evaluation_int = 0;

                // Checkmate handling
                if (evaluation[0] == '#') {
                    char sign = evaluation[1];

                    if (sign == '+') {
                        evaluation_int = 2000; // Positive checkmate
                    }
                    else if (sign == '-') {
                        evaluation_int = -2000; // Negative checkmate
                    }
                    else {
                        std::cerr << "Invalid sign after #." << std::endl;

                    }
                }
                else {
                    evaluation_int = std::stoi(evaluation);
                }

                counter++;
                evaluation_int = std::clamp(evaluation_int,-2000,2000);
                if (counter <= 50000 - gamesStored) {
                  
                    for (int k = 0; k < nbInput; k++)
                    {
                        input[idx][k] = board_state[k];
                    }
                    output[idx][0] = std::tanh(evaluation_int);
                    idx++;
                }
                else {
                    storedBoards.insert(storedBoards.end(), board_state.begin(), board_state.end());
                    storedBoards.push_back(std::tanh(evaluation_int));
                }

            }
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
            int lineSize = nbOutput+nbInput;
            inputsize = data.size()/lineSize;
            input = new float*[inputsize];
            output = new float*[inputsize];
            for (int i = 0; i < inputsize; i++)
            {
                input[i] = new float[nbInput];
                output[i] = new float[nbOutput];
                for (int j = 0; j < lineSize; j++)
                {
                    if(j < lineSize - nbOutput) input[i][j] = data[i*lineSize + j];
                    else output[i][j-(nbInput)] = data[i*lineSize + j];
                }
            }
        }

        if(storedBoards.size()){
            if(inputBoards != nullptr) delete inputBoards;
            if(outputBoards != nullptr) delete outputBoards;
            int lineSize = nbOutput+nbInput;
            inputsizeboard = storedBoards.size()/lineSize;
            inputBoards = new float*[inputsizeboard];
            outputBoards = new float*[inputsizeboard];
            for (int i = 0; i < inputsizeboard; i++)
            {
                inputBoards[i] = new float[nbInput];
                outputBoards[i] = new float[nbOutput];
                for (int j = 0; j < lineSize; j++)
                {
                    if(j < lineSize - nbOutput) inputBoards[i][j] = storedBoards[i*lineSize + j];
                    else outputBoards[i][j-(nbInput)] = storedBoards[i*lineSize + j];
                }
            }
        }

        ImGui::EndCombo();
    }
}

void MiniMLDisplay::PlotClassification(){
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

void MiniMLDisplay::PlotRegression(){
    if(network != nullptr && updateHeat){
        
        ImPlot::PushColormap(ImPlotColormap_Jet);

        ImS8 x[2] = {0.0f,0.0f};
        ImS8 y[2] =  {0.0f,0.0f};

        if (Plot){
            static float xs1[1001], ys1[1001];
            for (int i = 0; i < 1001; ++i) {
                xs1[i] = i * 0.001f;
                 if(this->type == NetworkType::RBF){
                    ys1[i] = MiniML::RBFSimulate(network,&xs1[i],1,0.1f)[0];
                }
                else{
                    ys1[i] = MiniML::SimulateNetwork(network,&xs1[i],1)[0];
                }
                
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


void MiniMLDisplay::RenderMiniML(){
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
    if(NetworkVisualiseur){
        DisplayNetwork(network);
        ImGui::SameLine();
    }

    if(regression){
        PlotRegression();

        if(Trainning && network != nullptr){
            if(type == NetworkType::MLP){
                MiniML::BackPropagation(network,input,inputsize,output,inputsize,learningRate,interationMax);
                updateHeat = true;
            }
            else if(type == NetworkType::Linear ){
                MiniML::LinearPropagation(network,input,inputsize,output,inputsize,learningRate,interationMax);
                updateHeat = true;
            }
            else if(type == NetworkType::RBF ){
                MiniML::RBFPropagation(network,input,inputsize,nbInput,output,inputsize,learningRate,this->kvalue,interationMax);
                updateHeat = true;
            }
        }
        

    }else{
        if(Trainning && network != nullptr){

            if(type == NetworkType::MLP){
                MiniML::BackPropagation(network,input,inputsize,output,inputsize,learningRate,interationMax);
                if(Plot && !regression){
                    heatMapMiniML = UpdateHeatMap(network,sizex,sizey,heatMapMiniML);
                    updateHeat = true;
                }
            }
            else if(type == NetworkType::Linear){
                MiniML::LinearPropagation(network,input,inputsize,output,inputsize,learningRate,interationMax);
                if(Plot && !regression){
                    heatMapMiniML = UpdateHeatMap(network,sizex,sizey,heatMapMiniML);
                    updateHeat = true;
                }
            }
            else if(type == NetworkType::RBF){
                MiniML::RBFPropagation(network,input,inputsize,nbInput,output,inputsize,learningRate,this->kvalue,interationMax);
                if(Plot && !regression){
                    heatMapMiniML = UpdateHeatMap(network,sizex,sizey,heatMapMiniML);
                    updateHeat = true;
                }
            }
        }

        PlotClassification();
    }


}