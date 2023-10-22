#include "../Headers/RenderContextDisplay.h"

#include <string>
#include <iostream>
bool RenderContextDisplay::DisplayRenderWindow(int width,int height,Scene* scene,char* label){
       
    bool selected =false;

        if(!ImGui::Begin(label,&p_open,ImGuiWindowFlags_NoCollapse)){
                ImGui::End();
        }
        else{


            static ImGuiComboFlags flags = ImGuiComboFlags_NoPreview;
            static const char* item_current = renderContextes[0]->GetLabel();
            
            if (ImGui::BeginCombo("Render Context", item_current, flags)) 
            {
                for (int n = 0; n < renderContextes.size(); n++)
                {
                    bool is_selected = (item_current == renderContextes[n]->GetLabel());
                    if (ImGui::Selectable(renderContextes[n]->GetLabel(), is_selected)){
                        item_current = renderContextes[n]->GetLabel();
                        idx_renderContext = n;
                    }
                        
                    if (is_selected){
                        ImGui::SetItemDefaultFocus();  
                    }
                        
                }
                ImGui::EndCombo();
            }

            RenderContext* renderContext = renderContextes[idx_renderContext];
            ImVec2 size = ImGui::GetWindowSize();
            renderContext->UpdateRender(size.x,size.y);
            MVP[1] = glm::perspectiveFov(glm::radians(45.0f), size.x, size.y, 0.5f, 1000.0f);
            unsigned int tex =  renderContext->RenderScene(GetTransform(),scene);

            ImGui::BeginChild("Game Render");
        
            ImGui::Image((void*)(intptr_t)tex,ImVec2(size.x,size.y-110),ImVec2(0,1 ),ImVec2(1,0 ));
            if(ImGui::IsWindowFocused()){
              selected = true;
            }
            ImGui::EndChild();

            
            
            if(ImGui::IsWindowFocused()){
              selected = true;
            }


            ImGui::End();
        }


    return selected;
}