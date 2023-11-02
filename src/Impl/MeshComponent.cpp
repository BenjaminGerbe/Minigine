#include "../Headers/MeshComponent.h"

void MeshComponent::Editor(){

    if (ImGui::BeginCombo("Select Mesh", obj->GetMesh()->GetName())) {
        for (int n = 0; n < projet->GetPrimitive().size(); n++)
        {
            char nbuffer[32];
            strcpy(nbuffer,projet->GetMesh(n)->GetName());
        
            if (ImGui::Selectable(nbuffer)){
                obj->SetMesh(projet->GetMesh(n));
            }

        }
        ImGui::EndCombo();
    }

    return;
};

void MeshComponent::SetUp(){
    projet = obj->GetProjet();

    return;
}

void MeshComponent::Update(){
   
    return;
}