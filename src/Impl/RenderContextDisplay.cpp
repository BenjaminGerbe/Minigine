#include "../Headers/RenderContextDisplay.h"

#include <string>
#include <iostream>
void RenderContextDisplay::DisplayRenderWindow(int width,int height,Scene* scene,char* label){
       
       
        ImGui::SetNextWindowSize(ImVec2(width,height));
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
            unsigned int tex =  renderContext->RenderScene(scene);

            ImGui::BeginChild("Game Render");
            ImVec2 size = ImGui::GetWindowSize();
            ImGui::Image((void*)(intptr_t)tex,size,ImVec2(0,1 ),ImVec2(1,0 ));
            ImGui::EndChild();

            ImGui::End();
        }
}