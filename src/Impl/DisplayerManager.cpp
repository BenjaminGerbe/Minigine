#include "../Headers/DisplayerManager.h"

bool isToDelete(RenderContextDisplay *element){
    return !element->GetOpen();
}



void DisplayerManager::RenderAllRenderWindows(int width,int height,Scene* scene){

    std::vector<RenderContextDisplay*>::iterator it = RenderContextDisplays.begin();
    int idx=0;
    while(it != RenderContextDisplays.end()){
        if( isToDelete(*it)){
            delete *it;
            it = RenderContextDisplays.erase(it);
        }
        else{
            char buffer[50]; 
            char* str = "Scene View ";
            sprintf(buffer, "%s%d", str,idx);
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
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
}