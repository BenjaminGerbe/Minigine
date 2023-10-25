#include "../Headers/Camera.h"

void CameraComp::Editor(){

    char* selected = "Orthographique";
    if(RenderMode == 0 ){
        selected = "Perspective";
    }

    if(ImGui::BeginCombo("Render Type",selected)){
        if(ImGui::Selectable("Perspective")){
          RenderMode = 0;
        }

        if(ImGui::Selectable("Orthographique")){
            RenderMode = 1;
        }

        ImGui::EndCombo();
    }

    ImGui::Spacing();
    if(RenderMode == 0){
        ImGui::DragFloat("Fov",&fov);
    }
    else{
        ImGui::DragFloat("Size",&size);
    }
   
};

void CameraComp::SetUp(){
    return;
}

void CameraComp::Update(){
    return;
}