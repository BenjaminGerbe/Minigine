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

            ImGui::SameLine(ImGui::GetWindowWidth()-200);

            if (ImGui::BeginCombo("Render Type","Perspective",flags)) 
            {
                if (ImGui::Selectable("Perspective")){
                    renderType = Perspective;
                }

                if (ImGui::Selectable("Orthographique")){
                    renderType = Orthographic   ;
                }

                ImGui::EndCombo();
            }

            RenderContext* renderContext = renderContextes[idx_renderContext];
            ImVec2 size = ImGui::GetWindowSize();
            renderContext->UpdateRender(size.x,size.y);

            if(renderType == Perspective){
                MVP[1] = glm::perspectiveFov(glm::radians(perspectiveFov), size.x, size.y, nearClip, farClip);
            }
            else{
                float ratio = size.x/size.y;
                float w =  ratio*OrthographiqueSize;
                float h =  OrthographiqueSize;
                MVP[1] =  glm::ortho( -w/2.0f, w/2.0f,-h/2.0f, h/2.0f, nearClip,farClip);;
            }

            unsigned int tex =  renderContext->RenderScene(GetTransform(),scene);

            ImGui::BeginChild("Game Render");
        
            ImGui::Image((void*)(intptr_t)tex,ImVec2(size.x,size.y-110),ImVec2(0,1 ),ImVec2(1,0 ));
            if(ImGui::IsWindowHovered()){
              selected = true;
            }
            ImGui::EndChild();

            
            
            if(ImGui::IsWindowHovered()){
              selected = true;
            }


            ImGui::End();
        }


    return selected;
}