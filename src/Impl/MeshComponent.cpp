#include "../Headers/MeshComponent.h"
RTTI_DEFINITION_BASE(MeshComponent,MeshComponent)
void MeshComponent::Editor(){

    if (ImGui::BeginCombo("Select Mesh", obj->GetMesh()->GetName().c_str())) {
        for (int n = 0; n < projet->GetPrimitive().size(); n++)
        {
            char nbuffer[255];
            strcpy(nbuffer,projet->GetMesh(n)->GetName().c_str());
        
            if (ImGui::Selectable(nbuffer)){
                obj->SetMesh(projet->GetMesh(n));
            }
        }
        ImGui::EndCombo();
    }

    
    if(ImGui::Button("Delete##MeshDelete")){

        int selectedMesh = 0;
        while(selectedMesh < projet->GetMeshPrimive().size()){
            if(projet->GetMesh(selectedMesh) == obj->GetMesh()){
                break;
            }else{
                selectedMesh++;
            }
        }
        projet->deletMesh(selectedMesh);
        obj->SetMesh(projet->GetMesh(0));
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