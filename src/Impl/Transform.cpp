#include "../Headers/Transform.h"

void Transform::Editor(){
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0.0f,15.0f));
    float static pos[3];
    float static rot[3];
    float static sca[3];
    pos[0] = obj->GetPosition().x; pos[1] = obj->GetPosition().y; pos[2] = obj->GetPosition().z;
    rot[0] = obj->GetRotation().x; rot[1] = obj->GetRotation().y; rot[2] = obj->GetRotation().z;
    sca[0] = obj->GetScale().x; sca[1] = obj->GetScale().y; sca[2] = obj->GetScale().z;

    ImGui::DragFloat3("Position",pos,0.1f);
    ImGui::DragFloat3("Rotation",rot,0.1f);
    ImGui::DragFloat3("Scale",sca,0.1f);
    ImGui::PopStyleVar(1);
    obj->SetPosition(glm::vec3({pos[0],pos[1],pos[2]}));
    obj->SetRotation(glm::vec3({rot[0],rot[1],rot[2]}));
    obj->SetScale(glm::vec3({sca[0],sca[1],sca[2]}));
};

void Transform::SetUp(){
    return;
}

void Transform::Update(){
   
    return;
    // glm::vec3 rot = obj->GetRotation();
    // rot.y += .1f;
    // if(rot.y > 360.0f){
    //     rot.y = 0.0f;
    // }
    // obj->SetRotation(rot);
   
}