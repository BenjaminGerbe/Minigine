#include "../Headers/DisplayerManager.h"

bool isToDelete(RenderContextDisplay *element){
    return !element->GetOpen();
}

void DisplayerManager::MachineState(Projet* projet){
    if(!openMachineState) return;
        ImGui::Begin("Profiler",&openMachineState);
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;

        DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
        SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
        
        float max = totalVirtualMem;
        float value  = virtualMemUsedByMe;
        value /= 1000000.0f;
        memoryUsage.push_back(value);
        memoryUsage.erase(memoryUsage.begin());
        char buffer[64];
        int ret = snprintf(buffer, sizeof buffer, "%f", value);
        ImGui::PushStyleColor(ImGuiCol_PlotLines,ImVec4(0.98f,0.60f,0.10f,1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg,ImVec4(0.5f,0.5f,0.5f,0.5f));

        ImGui::PlotLines("Memory Usage",&memoryUsage[0],500,0,buffer,0,150.0f,ImVec2(0,65.0f));

        ApplicationState* appState = projet->getAppState();
        double currentTime = appState->GetGlfTime();
        
        appState->SetFrames(appState->GetFrames()+1);
        float fps = fpsCounter[0];
        if ( currentTime -appState->GetLastTime() >= 1.0 ){ // If last prinf() was more than 1 sec ago
 
            appState->SetLastTime(appState->GetLastTime()+1);
            fpsCounter.push_back(1000.0f/((float)appState->GetFrames()));
            fpsCounter.erase(fpsCounter.begin());
            fps = appState->GetFrames();
            appState->SetFrames(0);
        }

        char bufferFps[64];
        ret = snprintf(bufferFps, sizeof (bufferFps), "%f",  fpsCounter[fpsCounter.size()-1]);
        ImGui::PlotLines("FrameTime (ms)",&fpsCounter[0],25,0,bufferFps,0.0f,120.0f,ImVec2(0,65.0f));
        ImGui::PopStyleColor(2);

    ImGui::End();
}

void DisplayerManager::RenderGameView(GameView* GameView,Projet* projet){
    ImGui::Begin("GameView");
    Scene* scene = projet->GetScene();
    ApplicationState* appState = projet->getAppState();
    if(scene->GetCameras().size() > 0){
        RenderContext* renderContext = rcGameView;
        
        ImVec2 size = ImGui::GetWindowSize();

        int width =  appState->GetWidth();
        int height =  appState->GetHeight();

        if(width != size.x || height != size.y-65){
            appState->SetWidth(size.x);
            appState->SetHeight(size.y-65);
            renderContext->UpdateRender(width,height);
        }
      
        CameraComp* camera = scene->GetCameras()[0];
        unsigned int tex =  renderContext->RenderScene(camera->GetMVP(width,height),scene);
        ImGui::BeginChild("Game Render");
        ImGui::Image((void*)(intptr_t)tex,ImVec2(width,height),ImVec2(0,1 ),ImVec2(1,0 ));
        ImGuiIO& io = ImGui::GetIO();
        float offsetX = ImGui::GetCursorScreenPos().x;
        float offsetY = ImGui::GetCursorScreenPos().y;

        if( ImGui::IsWindowHovered()){
            projet->getAppState()->SetMousePosition(glm::vec2({io.MousePos.x - offsetX,offsetY - io.MousePos.y }));
            projet->getAppState()->SetMVP(camera->GetMVP(width,height));
        }

        projet->getAppState()->SetGameViewHovered(ImGui::IsWindowHovered() );

        ImGui::EndChild();
    }

    ImGui::End();
}

void DisplayerManager::ObjectEditor(Projet* projet){
   
    if(!openObjectView)
        return;

    Scene* scene = projet->GetScene();
    ImGui::Begin("Object View",&openObjectView);

        if(selectedObjects >= 0 && scene->GetObjects().size() > 0 && selectedObjects < scene->GetObjects().size() ){
        
            Object* obj = scene->GetObjects()[this->selectedObjects];
            char buff[16];
          
            std::strcpy(buff,obj->GetName());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0.0f,15.0f));
            ImGui::Spacing();
            if(ImGui::InputText("##Name",buff,IM_ARRAYSIZE(buff),0)){
                obj->SetName(std::string(buff));
            }
            ImGui::PopStyleVar(1);
            float color[4];
            color[0] = obj->GetColor().x;
            color[1] = obj->GetColor().y;
            color[2] = obj->GetColor().z;
            color[3] = obj->GetColor().w;
            unsigned int flags;
            ImGui::SameLine();
            ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel );
            obj->SetColor(ImVec4(color[0],color[1],color[2],color[3]));
            ImGui::SetCursorPosY(ImGui::GetCursorPosY()+10.0f);    
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.0,0.0,0.0,0.0));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(0.0,(ImGui::GetFrameHeight()-35.0)/2.0f));
            static bool closeModalPopupTex = false;
            if(ImGui::ImageButton((void*)(intptr_t)obj->GetTexId(),ImVec2(35,35))){
                ImGui::OpenPopup("ImagePopUp");
                closeModalPopupTex = true;
            }
        

            if (ImGui::BeginPopupModal("ImagePopUp",&closeModalPopupTex))
            {
                for (int i = 0; i < projet->getTexID().size(); i++)
                {
                    if(ImGui::ImageButton((void*)(intptr_t)projet->getTexID()[i],ImVec2(50,50))){
                        obj->SetTexID(projet->getTexID()[i]);
                        closeModalPopupTex = false;
                    }
                }
                
                ImGui::EndPopup();
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            auto col = IM_COL32(54, 54, 54, 204);
            for (size_t i = 0; i < obj->GetComponents().size(); i++)
            {
                Component* c = obj->GetComponents()[i];
                ImGui::SetNextItemOpen(c->GetOpen());
                c->SetOpen(false);
                ImGui::AlignTextToFramePadding();
                float size = ImGui::GetWindowWidth();
                bool treeopen = ImGui::TreeNodeEx(c->GetHeaderName(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed);
                ImVec2 pos = ImVec2(ImGui::GetItemRectMin().x+size-50.0f,ImGui::GetItemRectMin().y+12.0f);
               
                
                if(c->GetID() != c_Transform){
                    if(ImGui::CloseButton(ImGui::GetID(c->GetHeaderName()),pos)){
                        obj->DeleteComponent(i);
                        ImGui::TreePop();
                        continue;
                    }
                }
                

                if (treeopen)
                {
                    c->Editor();
                    c->SetOpen(true);
                    ImGui::TreePop();
                }


            }

            ImGui::Separator();

            if (ImGui::BeginCombo("Add Component", "", ImGuiComboFlags_NoPreview)) 
            {
                for (int n = 0; n < projet->GetComponents().size(); n++)
                {
                    char nbuffer[32];
                    strcpy(nbuffer,projet->GetComponents()[n]->GetHeaderName());
                
                    if (ImGui::Selectable(nbuffer)){
                        Component* comp = projet->GetComponents()[n]->Clone(obj);
                        comp->SetUp();
                        obj->AddComponent(comp);

                    }

                }
                ImGui::EndCombo();
            }
            
        }  
    ImGui::End();

}
static int numOfHeapAllocations=0;
void* operator new(size_t size)
{
    numOfHeapAllocations++;
    return malloc(size);
}

void DisplayerManager::SceneEditor(Projet* projet){
    
    if(!openSceneEditor)
        return;

    ImGui::Begin("Scene Editor",&openSceneEditor,ImGuiWindowFlags_NoScrollbar);

    Scene* scene = projet->GetScene();
    std::vector<Object*> objets = projet->GetObjs();
    std::vector<Object*> ObjetsScene = scene->GetObjects();
    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
    ImGuiIO& io = ImGui::GetIO();
    int sizeY = ImGui::GetWindowSize().y;

    if(ImGui::IsKeyDown(ImGuiKey_Delete) && selectedObjects >= 0 && selectedObjects < scene->GetObjects().size()){
        scene->RemoveObjectScene(selectedObjects);
        if(selectedObjects < scene->GetObjects().size()-1){
            selectedObjects = scene->GetObjects().size()-1;
        }    
        else{
            selectedObjects = -1;
        }    
    
    }

    static bool isDrag  = false;
    static int dragItem = -1;
    static int hoverItem = -1;
    bool itemHover = false;
    static ImVec2 dragStartPos = io.MousePos;
    static ImVec2 p_min;
    static ImVec2 p_max;

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered,ImVec4(0.0f,0.0f,0.0f,0.0f));
    ImGui::PushStyleColor(ImGuiCol_Header,ImVec4(0.24f,0.51f,0.72f,0.5f));
    ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt,ImVec4(0.1f,0.1f,0.1f,0.9f));
    static auto col = IM_COL32(54, 54, 54, 204);
    float hCell = 30.0f;
    if (ImGui::BeginTable("editortab", 1,ImGuiTableFlags_RowBg))
    {
        static int item_current_idx = 0;

        for (int i = 0; i < scene->GetObjects().size(); i++)
        {   

            int popCount= 0;
            char buff[16];
            std::strcpy(buff,scene->GetObjects()[i]->GetName());

            char label[32];
            sprintf(label, "%s##%d",buff,i);

           

            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            const bool is_selected = (item_current_idx == i);

            ImGui::PushStyleColor(ImGuiCol_Text,scene->GetObjects()[i]->GetColor());

            if(hoverItem == i){
                col = IM_COL32(54, 54, 54, 204);
                if(is_selected ){
                    col = IM_COL32(63, 131, 186, 125);
                }

                ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max,col);
            }
            ImGui::Image((void*)(intptr_t)scene->GetObjects()[i]->GetTexId(),ImVec2(20,20));            
            ImGui::SameLine();
            if(ImGui::Selectable(label, is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap)){
                item_current_idx = i;
                selectedObjects = i;
              
            }
            
            if(hoverItem == i){
                p_min = ImGui::GetItemRectMin();
                p_max = ImGui::GetItemRectMax();
            }
          
            ImGui::PopStyleColor(1);
            if(ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(),ImGui::GetItemRectMax())){
                itemHover = true;
                hoverItem = i;
            }
    
            if(ImGui::IsItemClicked(0)){
                isDrag = true;
                dragStartPos = io.MousePos;
                dragItem = i;
                item_current_idx = i;
                selectedObjects = i;
            }

            if(ImGui::IsMouseReleased(0) && isDrag && itemHover){
                scene->SwapItem(i,dragItem);
                ImGui::ResetMouseDragDelta();
                isDrag = false;
                item_current_idx = i;
                selectedObjects = i;
            }

           ImGui::SameLine();

            char poly[64];
            int ret = snprintf(poly, sizeof poly, "%d", scene->GetObjects()[i]->GetMesh()->TriangleToDraw()/(sizeof(unsigned int)*3));
            ImGui::Text(poly);

       
        }



        ImGui::Text("");
        hCell = ImGui::GetTextLineHeight();
        int completeLine = std::floor(((sizeY) /hCell)  - (scene->GetObjects().size()));
        completeLine = (((0) > (completeLine)) ? (0) : (completeLine));
        for (int i = 0; i <completeLine; i++)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("");
        } 

        ImGui::EndTable();
       
    }
    ImGui::PopStyleColor(3);
    ImGui::End();
};

void DisplayerManager::AddRenderContextDisplay(RenderContextDisplay* renderWindow){
        RenderContextDisplays.push_back(renderWindow);

        renderWindow->AddRender(new RenderContextShaded());
        renderWindow->GetRenderContextes()[0]->SetLabel("Shaded");
        renderWindow->AddRender(new RenderContextShadedWireFrame());
        renderWindow->GetRenderContextes()[1]->SetLabel("ShadedWireFrame");
        renderWindow->AddRender(new RenderContextWireFrame());
        renderWindow->GetRenderContextes()[2]->SetLabel("WireFrame");

        std::cout << " je t'en suplie change ca " << std::endl;
        for (int i = 0; i < renderWindow->GetRenderContextes().size(); i++)
        {
            renderWindow->GetRenderContextes()[i]->SetUp(1920,1080);
        }
};


void DisplayerManager::RenderAllRenderWindows(int width,int height,Projet* projet){

    std::vector<RenderContextDisplay*>::iterator it = RenderContextDisplays.begin();
    int idx=0;
  
    while(it != RenderContextDisplays.end()){
        if( isToDelete(*it)){
            delete *it;
            it = RenderContextDisplays.erase(it);
        }
        else{
            char buffer[32]; 
            char* str = "Scene View## ";
            sprintf(buffer, "%s%d", str,idx);
            float w = ImGui::GetWindowSize().x;
            float h = ImGui::GetWindowSize().y;
            bool selected = RenderContextDisplays[idx]->DisplayRenderWindow(width,height,projet->GetScene(),buffer);
            
            if(selected){
                selectedSceneView = idx;
                ImGuiIO& io = ImGui::GetIO();

                if(ImGui::IsMouseDown(1) ){
                    projet->getAppState()->LockMouse(true);

                    RenderContextDisplay* obj = RenderContextDisplays[selectedSceneView];

                    float angleX = io.MouseDelta.x;
                    angleX = std::clamp(angleX,-10.0f,10.00f);

                    float angleY = io.MouseDelta.y;
                    angleY = std::clamp(angleY,-10.0f,10.00f);

                    glm::vec3 rot = obj->GetRotation();
                    rot.y += angleX;
                    rot.x += angleY;
                    obj->SetRotation(rot);

                    glm::vec3 pos = obj->GetPosition();
                    glm::vec3 pad = obj->GetPadding();
                    float middleMouse = io.MouseWheel;
                    pos.z -= middleMouse;

                    if(obj->GetRenderType() == Orthographic){
                        obj->SetOrthoSize(obj->GetOrthoSize() - middleMouse);
                        pos.z = obj->GetOrthoSize();
                    }

                    if(ImGui::IsKeyDown(ImGuiKey_Z)){
                         pos.z -= 0.05f;
                      
                    }

                    if(ImGui::IsKeyDown(ImGuiKey_S)){
                        pos.z += 0.05f;
                    }

                    
                    if(ImGui::IsKeyDown(ImGuiKey_Q)){
                        pad -= obj->GetRight() *0.05f;
                      
                    }

                    if(ImGui::IsKeyDown(ImGuiKey_D)){
                        pad += obj->GetRight() *0.05f;;
                      
                    }

                    obj->SetPosition(pos);
                    obj->SetPadding(pad);

                }else{
                    projet->getAppState()->LockMouse(false);  
                }
                
            }

            idx++;
            it++;
        }
    }
}

void DisplayerManager::MiniMLWindows(){
    for (int i = MLDisplays.size()-1; i >=0 ; i--)
    {
        if(!MLDisplays[i]->GetOpen()){
            delete MLDisplays[i]; 
            MLDisplays.erase(MLDisplays.begin()+i);
        }
        else{
            MLDisplays[i]->RenderMiniML();
        }
    }
    
}

void DisplayerManager::RenderSceneViewOption(){
    if(!openSceneViewOption)
        return;

    ImGui::Begin("Scene View Option",&openSceneViewOption);
    if(selectedSceneView >= 0 && selectedSceneView < RenderContextDisplays.size()){
        float static pos[3];
        float static rot[3];
        RenderContextDisplay* obj = RenderContextDisplays[selectedSceneView];
        pos[0] = obj->GetPosition().x; pos[1] = obj->GetPosition().y; pos[2] = obj->GetPosition().z;
        rot[0] = obj->GetRotation().x; rot[1] = obj->GetRotation().y; rot[2] = obj->GetRotation().z;
        float OrthoSize =obj->GetOrthoSize();
        float Fov =obj->GetFov();
        float nar[2];
        ImGui::SetNextItemOpen(true);

        nar[0] = obj->GetNearClip();
        nar[1] = obj->GetFarClip();
        
        if (ImGui::CollapsingHeader("Transform##trans")){
            ImGui::DragFloat3("Position",pos,0.1f);
            ImGui::DragFloat3("Rotation",rot,0.1f);
        }

        ImGui::SetNextItemOpen(true);

        if (ImGui::CollapsingHeader("Settings##sett")){
            if(obj->GetRenderType() == Orthographic){
               
                if(ImGui::DragFloat("Orthographic Size",&OrthoSize)){
                    obj->SetOrthoSize(OrthoSize);
                }     
            }
            else{
                if(ImGui::DragFloat("Fov",&Fov) && Fov > 0){
                    obj->SetFov(Fov);
                }   
            }

            if(ImGui::DragFloat2("Near/Far Clip",nar,0.1f)){
                obj->SetNearClip(nar[0]);
                obj->SetFarClip(nar[1]);
            }    
           
        }
        
        obj->SetPosition(glm::vec3({pos[0],pos[1],pos[2]}));
        obj->SetRotation(glm::vec3({rot[0],rot[1],rot[2]}));
    }
    ImGui::End();
}

void DisplayerManager::RenderAppOptions(Projet* projet){
       
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,6);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(3.0f,3.0f));
        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.9f, 0.94f, 0.97f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(0.9f, 0.94f, 0.97f, 1.00f));
      
        std::vector<Object*> objects = projet->GetObjs();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", "CTRL+S")) {
                    Saver::SaveScene(projet->GetScene());
                }
                if (ImGui::MenuItem("Import")) {
                    Saver::LoadScene(projet);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                if (ImGui::MenuItem("Scene View",NULL)) {
                    AddRenderContextDisplay(new RenderContextDisplay());
                }
                if (ImGui::MenuItem("Machine State",NULL)) {
                    openMachineState = true;
                }
                if (ImGui::MenuItem("Scene Editor",NULL)) {
                    openSceneEditor = true;
                }
                if (ImGui::MenuItem("Object View",NULL)) {
                    openObjectView = true;
                }
                if (ImGui::MenuItem("Scene View Option",NULL)) {
                    openSceneViewOption = true;
                }
                
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Shader"))
            {
                if (ImGui::MenuItem("Recompile",NULL)) {
                    for(auto mat : projet->GetMaterials()){
                        mat->GetShader()->Compile();
                    }
                }
                
                ImGui::EndMenu();
            }



            if (ImGui::BeginMenu("Objects"))
            {

                for (int n = 0; n < projet->GetObjs().size(); n++)
                {
                    char nbuffer[32];
                    strcpy(nbuffer,objects[n]->GetStrName().c_str());

                    if (ImGui::MenuItem(nbuffer,NULL)) {
                        Object* obj = new Object(*objects[n]);
                        projet->GetScene()->AddObjectScene(obj);
                        obj->SetProjet(projet);
                        obj->SetUp();
                    }
                }

                ImGui::EndMenu();
            }

            
            if (ImGui::BeginMenu("MiniML"))
            {
                if (ImGui::BeginMenu("MLP"))
                {
                    if (ImGui::MenuItem("Classification",NULL)) {
                        int id = 0;
                        if(MLDisplays.size() >0) id = MLDisplays[MLDisplays.size()-1]->GetID();
                        MLDisplays.push_back(new MiniMLDisplay(false,NetworkType::MLP,id+1,projet));
                    }

                    if (ImGui::MenuItem("Regression",NULL)) {
                        int id = 0;
                        if(MLDisplays.size() >0) id = MLDisplays[MLDisplays.size()-1]->GetID();
                        MLDisplays.push_back(new MiniMLDisplay(true,NetworkType::MLP,id+1,projet));
                    }
                    
                    ImGui::EndMenu();
                }
                
                if (ImGui::BeginMenu("Linear"))
                {
                    if (ImGui::MenuItem("Classification",NULL)) {
                        int id = 0;
                        if(MLDisplays.size() >0) id = MLDisplays[MLDisplays.size()-1]->GetID();
                        MLDisplays.push_back(new MiniMLDisplay(false,NetworkType::Linear,id+1,projet));
                    }

                    if (ImGui::MenuItem("Regression",NULL)) {
                        int id = 0;
                        if(MLDisplays.size() >0) id = MLDisplays[MLDisplays.size()-1]->GetID();
                        MLDisplays.push_back(new MiniMLDisplay(true,NetworkType::Linear,id+1,projet));
                    }
                    
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("RBF"))
                {
                    if (ImGui::MenuItem("Classification",NULL)) {
                        int id = 0;
                        if(MLDisplays.size() >0) id = MLDisplays[MLDisplays.size()-1]->GetID();
                        MLDisplays.push_back(new MiniMLDisplay(false,NetworkType::RBF,id+1,projet));
                    }

                    if (ImGui::MenuItem("Regression",NULL)) {
                        int id = 0;
                        if(MLDisplays.size() >0) id = MLDisplays[MLDisplays.size()-1]->GetID();
                        MLDisplays.push_back(new MiniMLDisplay(true,NetworkType::RBF,id+1,projet));
                    }
                    
                    ImGui::EndMenu();
                }

                 if (ImGui::MenuItem("ChessVisualiseur",NULL)) {
                    int id = 0;
                    if(MLDisplays.size() >0) id = MLDisplays[MLDisplays.size()-1]->GetID();
                    MLDisplays.push_back(new MiniMLDisplay(true,NetworkType::ChessDisplayer,id+1,projet));
                 }


                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
}