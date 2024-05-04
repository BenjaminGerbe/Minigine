#include "../Headers/Light.h"
#include "../Headers/Projet.h"
#include "../Headers/Scene.h"
RTTI_DEFINITION_BASE(LightComp,LightComp)

void LightComp::Editor(){
    ImGui::Spacing();
    ImGui::DragFloat("Intensity",&this->intensity,0.1f);
    ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel;
    ImGui::Text("Color :");
    ImGui::ColorPicker3("##Color",this->color,flags);

};

void LightComp::SetUp(){
    return;
}

void LightComp::Update(){
    return;
}

LightComp::~LightComp(){
    obj->GetProjet()->GetScene()->GetRemoveLight(this);
    delete color;
}