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
    static float d;
    ImGui::DragFloat("Fov",&d);

   
};

void CameraComp::SetUp(){
    return;
}

void CameraComp::Update(){
    return;
}