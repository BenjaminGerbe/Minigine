#include "../Headers/RenderContext.h"
#include "../Headers/Scene.h"

void RenderContextShaded::RenderScene(Scene* scene){
    scene->Render(MVP);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDrawElements(GL_TRIANGLES,scene->VertexToRender(),GL_UNSIGNED_INT,(void*)0);
};

void RenderContextShaded::SetUp(glm::mat4* _MVP ){

    glGenBuffers(1,&UBO);
    glBindBuffer(GL_UNIFORM_BUFFER,UBO);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*2,nullptr,GL_STREAM_DRAW);
    MVP = _MVP;
};