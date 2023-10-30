#include "../Headers/RenderContextDisplay.h"
#include <string>
#include <iostream>

bool RenderContextDisplay::DisplayRenderWindow(int width,int height,Scene* scene,char* label){
       
    bool selected =false;

    if(!ImGui::Begin(label,&p_open,ImGuiWindowFlags_NoCollapse)){
        ImGui::End();
    }
    else{

        ImGuiComboFlags flags = ImGuiComboFlags_NoPreview;
        
        if (ImGui::BeginCombo("Render Context", item_current, flags)) 
        {
            for (int n = 0; n < renderContextes.size(); n++)
            {
                RenderContext* render = renderContextes[n];
                bool is_selected = (item_current == render->GetLabel());
                if (ImGui::Selectable(render->GetLabel(), is_selected)){
                    item_current = render->GetLabel();
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

        if(size.x != this->width || size.y != this->height){
            this->height = size.y;
            this->width = size.x;
            renderContext->UpdateRender(this->height,this->width);
        }
        
        if(renderType == Perspective){
            MVP[1] = glm::perspectiveFov(glm::radians(perspectiveFov), (float)this->width, (float)this->height, nearClip, farClip);
        }
        else{
            float ratio = this->width/this->height;
            float w =  ratio*OrthographiqueSize;
            float h =  OrthographiqueSize;
            MVP[1] =  glm::ortho( -w/2.0f, w/2.0f,-h/2.0f, h/2.0f, nearClip,farClip);;
        }

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

void RenderContextDisplay::AddRender(RenderContext* render){
    renderContextes.push_back(render);
    item_current = renderContextes[0]->GetLabel();
}

glm::mat4* RenderContextDisplay::GetTransform(){
    MVP[0] =  glm::translate(glm::mat4(1.f), -position);
    MVP[0] *= glm::rotate(glm::mat4(1.0f), rotation.x * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    MVP[0] *= glm::rotate(glm::mat4(1.0f), rotation.y * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    MVP[0] *= glm::rotate(glm::mat4(1.0f), rotation.z * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    MVP[0] *= glm::translate(glm::mat4(1.f), -padding);

    return MVP;
};