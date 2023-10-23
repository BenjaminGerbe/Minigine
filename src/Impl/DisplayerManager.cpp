#include "../Headers/DisplayerManager.h"

bool isToDelete(RenderContextDisplay *element){
    return !element->GetOpen();
}

void DisplayerManager::MachineState(){
    if(!openMachineState) return;
    ImGui::Begin("Machine State",&openMachineState);

    
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
        ImGui::PlotLines("Memory Usage",&memoryUsage[0],500,0,buffer,0,150.0f,ImVec2(0,65.0f));

    ImGui::End();
}

void DisplayerManager::ObjectEditor(Scene* scene){
   
    if(!openObjectView)
        return;

    ImGui::Begin("Object View",&openObjectView);

        if(selectedObjects >= 0 && scene->GetObjects().size() > 0){
        
            Object* obj = scene->GetObjects()[this->selectedObjects];
            char buff[16];
          
            std::strcpy(buff,obj->GetName());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0.0f,15.0f));
            ImGui::Spacing();
            if(ImGui::InputText("##Name",buff,IM_ARRAYSIZE(buff),0)){
                obj->SetName(std::string(buff));
            }
            ImGui::PopStyleVar(1);
            ImGui::Separator();
           
            for(Component* c : obj->GetComponents()){
                ImGui::SetNextItemOpen(c->GetOpen());
                c->SetOpen(false);
                if (ImGui::CollapsingHeader(c->GetHeaderName())){
                    c->Editor();
                    c->SetOpen(true);
                }
            }
            
        }  
    ImGui::End();

}

void DisplayerManager::SceneEditor(Scene* scene,std::vector<Object*> objets){

    if(!openSceneEditor)
        return;

    ImGui::Begin("Scene Editor",&openSceneEditor);

    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;


    if (ImGui::BeginCombo("Add Object", "", ImGuiComboFlags_NoPreview)) 
    {
        for (int n = 0; n < objets.size(); n++)
        {
            char nbuffer[32];
            strcpy(nbuffer,objets[n]->GetStrName().c_str());
           
            if (ImGui::Selectable(nbuffer)){
                
                Object* obj = new Object(objets[n]->GetMesh(),objets[n]->GetStrName(),objets[n]->GetObjectType());
                scene->AddObjectScene(obj);
            }

         
                
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    if(ImGui::Button("-") && selectedObjects >= 0 && scene->GetObjects().size() > 0){
        scene->RemoveObjectScene(selectedObjects);

        if(selectedObjects > scene->GetObjects().size()-1){
            selectedObjects = scene->GetObjects().size()-1;
        }        
    }


    if (ImGui::BeginTable("split2", 2,0 ))
    {
        static int item_current_idx = 0;
        for (int i = 0; i < scene->GetObjects().size(); i++)
        {   
            char buff[16];
            std::strcpy(buff,scene->GetObjects()[i]->GetName());

            char label[32];
            sprintf(label, "%s##%d",buff,i);
            
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            const bool is_selected = (item_current_idx == i);
            if(scene->GetObjects()[i]->GetObjectType() == ClassicObject){
                ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(0.85f,0.85f,0.85f,1.0f));
            }
            else if(scene->GetObjects()[i]->GetObjectType() == Light){
                ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(0.91f,0.96f,0.25f,1.0f));
            }
            
            if(ImGui::Selectable(label, is_selected, ImGuiSelectableFlags_SpanAllColumns)){
                item_current_idx = i;
                selectedObjects = i;
            }
            ImGui::PopStyleColor(1);
     
            ImGui::TableNextColumn();

            char poly[64];
            int ret = snprintf(poly, sizeof poly, "%d", scene->GetObjects()[i]->GetMesh()->TriangleToDraw()/(sizeof(unsigned int)*3));
            ImGui::Text(poly);
 
        }

        ImGui::EndTable();
    }
    
    ImGui::End();
};

void DisplayerManager::AddRenderContextDisplay(RenderContextDisplay* renderWindow){
        RenderContextDisplays.push_back(renderWindow);

        renderWindow->AddRender(new RenderContextShaded());
        renderWindow->getRenderContextes()[0]->SetLabel("Shaded");
        renderWindow->AddRender(new RenderContextShadedWireFrame());
        renderWindow->getRenderContextes()[1]->SetLabel("ShadedWireFrame");
        renderWindow->AddRender(new RenderContextWireFrame());
        renderWindow->getRenderContextes()[2]->SetLabel("WireFrame");

        std::cout << " je t'en suplie change ca " << std::endl;
        for (int i = 0; i < renderWindow->getRenderContextes().size(); i++)
        {
            renderWindow->getRenderContextes()[i]->SetUp(1920,1080);
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

                if(ImGui::IsMouseDown(1)){
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
                        std::cout << obj->GetOrthoSize() << std::endl;
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

                }
                else{
                    projet->getAppState()->LockMouse(false);    
                }
              
            }
            
            idx++;
            it++;
        }
    }
}

void DisplayerManager::RenderSceneViewOption(){
    if(!openSceneViewOption)
        return;

    ImGui::Begin("Scene View Option",&openSceneViewOption);
    if(selectedSceneView >= 0){
        float static pos[3];
        float static rot[3];
        RenderContextDisplay* obj = RenderContextDisplays[selectedSceneView];
        pos[0] = obj->GetPosition().x; pos[1] = obj->GetPosition().y; pos[2] = obj->GetPosition().z;
        rot[0] = obj->GetRotation().x; rot[1] = obj->GetRotation().y; rot[2] = obj->GetRotation().z;
        float OrthoSize =obj->GetOrthoSize();
        float Fov =obj->GetFov();
        float nar[2];

        nar[0] = obj->GetNearClip();
        nar[1] = obj->GetFarClip();
        
        if (ImGui::CollapsingHeader("Transform##trans")){
            ImGui::DragFloat3("Position",pos,0.1f);
            ImGui::DragFloat3("Rotation",rot,0.1f);
        }

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
            ImGui::EndMainMenuBar();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
}