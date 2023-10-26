#include "../Headers/LineRenderer.h"

void LineRenderer::Editor(){
    return;
}

void LineRenderer::SetUp(){
    projet = this->obj->GetProjet();
    targetMouse = new Object(projet->GetObjs()[0]->GetMesh(),projet->GetObjs()[0]->GetStrName(),projet->GetObjs()[0]->GetObjectType());
    projet->GetScene()->AddObjectScene(targetMouse);
    return;
}

void LineRenderer::Update(){
    glm::vec3 rot = targetMouse->GetRotation();
    rot.y +=.1f;
    targetMouse->SetRotation(rot);
    glm::mat4* MVP = projet->getAppState()->GetMVP();
    if(MVP == nullptr){
        return;
    }
    glm::vec2 Mouse = projet->getAppState()->GetMousePosition();
    float width = projet->getAppState()->GetWidth();
    float height = projet->getAppState()->GetHeight();
    Mouse.x = ((Mouse.x*2.0f)-width)/width;
    Mouse.y = ((Mouse.y*2.0f)-height)/height;
    glm::mat4 mat = MVP[1]*MVP[0];
    mat = glm::inverse(mat);
    glm::vec4 vector({Mouse.x,Mouse.y,0.0f,1.0f});
    glm::vec4 pos = mat*vector;
    
    pos.w = 1.0 / pos.w;
    pos.x *= pos.w;
    pos.y *= pos.w;
    pos.z = 0.0f;

    targetMouse->SetPosition(glm::vec3(pos));

   std::cout <<  Mouse.x << " " << Mouse.y  << std::endl;
    return;
}