#include "../Headers/Scene.h"
#include "../data.h"

void Scene1::SetUp(int width,int height){
        this->width = width;
        this->height = height;

        // create buffers
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);
        glGenVertexArrays(1, &VAO);  
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(verticesArray),verticesArray,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indicesArray),indicesArray,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1,&UBO);
        glBindBuffer(GL_UNIFORM_BUFFER,UBO);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*2,nullptr,GL_STREAM_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER,0,UBO);
        glBindVertexArray(0);
        // createShader
        GLShader g_basicShader;
        g_basicShader.LoadVertexShader("frag.glsl");
        g_basicShader.LoadFragmentShader("vs.glsl");
        g_basicShader.Create();

        programShader = g_basicShader.GetProgram();
}

void Scene1::Render(glm::mat4* _MVP){


        glBindVertexArray(VAO);  
        glUseProgram(programShader);
        glm::mat4 cameraView = _MVP[0];
        cameraView *=  glm::translate(glm::mat4(1.f), -glm::vec3({ -2.f, 0.f,0.f }));
        glm::mat4 matrices[2];
        matrices[0] =  cameraView;
        matrices[1] =  _MVP[1];
       
        glBindBuffer(GL_UNIFORM_BUFFER,UBO);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*2,matrices,GL_STREAM_DRAW);
        glDrawElements(GL_TRIANGLES,VertexToRender(),GL_UNSIGNED_INT,(void*)0);

        cameraView =  _MVP[0];
        cameraView *=  glm::translate(glm::mat4(1.f), -glm::vec3({ 2.f, 0.f,0.f }));

        matrices[0] =  cameraView;
        matrices[1] =  _MVP[1];
       
        glBindBuffer(GL_UNIFORM_BUFFER,UBO);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*2,matrices,GL_STREAM_DRAW);
        glDrawElements(GL_TRIANGLES,VertexToRender(),GL_UNSIGNED_INT,(void*)0);


}

int Scene1::VertexToRender(){
        return sizeof(indicesArray)/sizeof(unsigned int);
}
