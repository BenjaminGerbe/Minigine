#include "../Headers/MaterialComponent.h"
#include "../Headers/Projet.h"
void MaterialComp::Editor(){

    Projet* projet = obj->GetProjet();
    auto materials = projet->GetMaterials();

    if(ImGui::BeginCombo("Materials",obj->GetMaterial()->GetName())){

        for (int i = 0; i < materials.size(); i++)
        {
            if(ImGui::Selectable(materials[i]->GetName())){
                obj->SetMaterial(materials[i]);
            }
        }
        
        ImGui::EndCombo();
    }

   obj->GetMaterial()->Editor();

    return;
};

void MaterialComp::SetUp(){
    return;
}

void MaterialComp::Update(){
    return;
}