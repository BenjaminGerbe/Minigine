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
void CallBacks(){
    
}
void DisplayerManager::ObjectEditor(Scene* scene){
    Object* obj = scene->GetObjects()[this->selectedObjects];
    
    ImGui::Begin("Object View");
    
    char buff[16];
    float static pos[3];
    float static rot[3];
    float static sca[3];
    pos[0] = obj->GetPosition().x; pos[1] = obj->GetPosition().y; pos[2] = obj->GetPosition().z;

    rot[0] = obj->GetRotation().x; rot[1] = obj->GetRotation().y; rot[2] = obj->GetRotation().z;

    sca[0] = obj->GetScale().x; sca[1] = obj->GetScale().y; sca[2] = obj->GetScale().z;

    std::strcpy(buff,obj->GetName());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0.0f,15.0f));
    ImGui::Spacing();
    ImGui::SetCursorPosX(135.0f);
    if(ImGui::InputText("##Name",buff,IM_ARRAYSIZE(buff),0)){
        obj->SetName(std::string(buff));
    }
    ImGui::Separator();
    ImGui::Text("Transformations");
    ImGui::DragFloat3("Position",pos,0.1f);
    ImGui::DragFloat3("Rotation",rot,0.1f);
    ImGui::DragFloat3("Scale",sca,0.1f);
    ImGui::PopStyleVar(1);
    obj->SetPosition(glm::vec3({pos[0],pos[1],pos[2]}));
    obj->SetRotation(glm::vec3({rot[0],rot[1],rot[2]}));
    obj->SetScale(glm::vec3({sca[0],sca[1],sca[2]}));

    ImGui::End();

}

void DisplayerManager::SceneEditor(Scene* scene){
    ImGui::Begin("Scene Editor");

    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
    
    if (ImGui::BeginTable("split2", 2,0 ))
    {
         static int item_current_idx = 0;
        for (int i = 0; i < scene->GetObjects().size(); i++)
        {   
            char buff[16];
            std::strcpy(buff,scene->GetObjects()[i]->GetName());

            char label[32];
            sprintf(label, "%d: %s",i,buff);
            
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            const bool is_selected = (item_current_idx == i);
            if(ImGui::Selectable(label, is_selected, ImGuiSelectableFlags_SpanAllColumns)){
                    item_current_idx = i;
                    selectedObjects = i;
            }
            ImGui::TableNextColumn();
            char poly[64];
            int ret = snprintf(poly, sizeof poly, "%d", scene->GetObjects()[i]->GetMesh()->TriangleToDraw()/(sizeof(unsigned int)*3));
            ImGui::Text(poly);
 
        }
        ImGui::EndTable();
    }
    
    ImGui::End();
};

void DisplayerManager::RenderAllRenderWindows(int width,int height,Scene* scene){

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
            RenderContextDisplays[idx]->DisplayRenderWindow(width,height,scene,buffer);
            idx++;
            it++;
        }
    }
}

void DisplayerManager::RenderAppOptions(){

       
       
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,6);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(3.0f,3.0f));
        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.9f, 0.94f, 0.97f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(0.9f, 0.94f, 0.97f, 1.00f));
      
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", "CTRL+S")) {}
                if (ImGui::MenuItem("Import")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                if (ImGui::MenuItem("Scene View",NULL)) {
                    AddRenderContextDisplay(new RenderContextDisplay(*this->renderContextDisplay));
                }
                if (ImGui::MenuItem("Machine State",NULL)) {
                    openMachineState = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
}