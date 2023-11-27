#include "../Headers/Scene.h"


unsigned int squareIndices[] = {
    // Face avant
    0, 1,2,
    2, 3,0
};

float SquareVertices[] = {

    -1.0f, -1.0f, 0.0f,  // Sommet 0
    1.0f, -1.0f, 0.0f,  // Sommet 1
    1.0f,  1.0f, 0.0f,  // Sommet 2
    -1.0f,  1.0f, 0.0f, 
};

float SquareUV[] = {
    0.0,0.0,
    1.0,0.0,
    1.0,1.0,
    0.0,1.0
};

void Scene::GetRemoveLight(LightComp* light){
    for (int i = 0; i < Lights.size(); i++)
    {
        if(Lights[i] == light){
            Lights.erase(Lights.begin()+i);
            break;
        }
    }
}


void Scene::GetRemoveCamera(CameraComp* cam ){
    for (int i = 0; i < Cameras.size(); i++)
    {
        if(Cameras[i] == cam){
            Cameras.erase(Cameras.begin()+i);
            break;
        }
    }
}



void Scene::SetUp(){


        unsigned int VBO,EBO;
        //Square
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);
        glGenVertexArrays(1, &VAOS);  
        glBindVertexArray(VAOS);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(SquareVertices),SquareVertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(squareIndices),squareIndices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        unsigned int VUV;
        glGenBuffers(1,&VUV);
        glBindBuffer(GL_ARRAY_BUFFER,VUV);
        glBufferData(GL_ARRAY_BUFFER,sizeof(SquareUV),SquareUV,GL_STATIC_DRAW);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        // UBO MVP
        glGenBuffers(1,&UBO);
        glBindBuffer(GL_UNIFORM_BUFFER,UBO);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*2,nullptr,GL_STREAM_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER,0,UBO);
     
        // createShader
        GLShader g_basicShader;
        g_basicShader.LoadVertexShader("frag.glsl");
        g_basicShader.LoadFragmentShader("vs.glsl");
        g_basicShader.Create();

        GLShader g_basicShaderWireFrame;
        g_basicShaderWireFrame.LoadVertexShader("frag.glsl");
        g_basicShaderWireFrame.LoadFragmentShader("FragWireFrame.glsl");
        g_basicShaderWireFrame.Create();

        GLShader g_basicShaderGrid;
        g_basicShaderGrid.LoadVertexShader("vsGrid.glsl");
        g_basicShaderGrid.LoadFragmentShader("FragGrid.glsl");
        g_basicShaderGrid.Create();

        programShaderWireFrame = g_basicShaderWireFrame.GetProgram();
        programShaderGrid = g_basicShaderGrid.GetProgram();
        programShader = g_basicShader.GetProgram();
}

void Scene::Render(glm::mat4* _MVP,int flags){

  
        for (int i = 0; i < Objects.size(); i++)
        {
                GLuint ID = Objects[i]->GetMesh()->GetVAO();
                GLuint shaderID = Objects[i]->GetMaterial()->GetShader()->GetIDX();
                glBindVertexArray(ID);  

                glUseProgram(shaderID);
                if(flags & WireFrame){
                    glUseProgram(Objects[i]->GetMaterial()->GetShader()->GetWireFrame());
                } 
              
                glm::mat4 cameraView = _MVP[0];
                cameraView *= Objects[i]->GetTransformation();
                glm::mat4 matrices[3];
                matrices[0] =  cameraView;
                matrices[1] =  _MVP[1];
                matrices[2] =  Objects[i]->GetTransformation();
                glBindBuffer(GL_UNIFORM_BUFFER,UBO);
                glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*3,matrices,GL_STREAM_DRAW);


                Objects[i]->GetMaterial()->ApplyParameter();

                if(Lights.size() > 0){
                    glUniform3fv(glGetUniformLocation(shaderID, "dir.direction"), 1, &Lights[0]->GetObj()->GetForward()[0]); 
                    glUniform3fv(glGetUniformLocation(shaderID, "dir.color"), 1, Lights[0]->GetColor()); 
                    glUniform1f(glGetUniformLocation(shaderID, "dir.intensity"),  Lights[0]->GetIntensity()); 
                }
                else{
                    glUniform1f(glGetUniformLocation(shaderID, "dir.intensity"),  0.0f);    
                }

               

                glDrawElements(GL_TRIANGLES,Objects[i]->GetMesh()->TriangleToDraw(),GL_UNSIGNED_INT,(void*)0);
        }
       
        if((flags & s_Grid_None))
            return;


        glBindVertexArray(VAOS);  
        glUseProgram(programShaderGrid);
        glm::mat4 cameraView =  _MVP[0];
        cameraView *= glm::scale(glm::mat4(1.0f),glm::vec3({50,1,50}));
        cameraView *= glm::rotate(glm::mat4(1.0f), 90 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
        cameraView *= glm::translate(glm::mat4(1.0f),glm::vec3({0.,0.,0.0}));
        static float t =0;
        t+=0.0005f;
        glm::mat4 matrices[3];
        matrices[0] =  cameraView;
        matrices[1] =  _MVP[1];
        matrices[2] =  glm::mat4(1.0);
        glBindBuffer(GL_UNIFORM_BUFFER,UBO);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*3,matrices,GL_STREAM_DRAW);
        glUniform1f(glGetUniformLocation(programShaderGrid, "time_"), t);    
     
        glDrawElements(GL_TRIANGLES,  sizeof(squareIndices)/sizeof(unsigned int),GL_UNSIGNED_INT,(void*)0);
}


void Scene::RemoveObjectScene(Object* object){
   int i =0;
   bool find = false;

   while(i < this->Objects.size() && !find){
        if(object == Objects[i]){
            find = true;
            this->RemoveObjectScene(i);
        }
        else{
            i++;
        }
   }
   
}

void Scene::SwapItem(int i, int k){
    std::swap(Objects[i],Objects[k]);
}

void Scene::RemoveObjectScene(std::vector<Object*> objects){
   int i = this->Objects.size()-1;

   while(i >= 0){
        
        for (int k = 0; k < objects.size(); k++)
        {
            if(objects[k] == Objects[i]){
            this->RemoveObjectScene(i);
            break;
            }
        }
       
        i--;
   }
   
}

void Scene::RemoveObjectScene(int idx){
    Object* o =  Objects[idx];
    Objects.erase(Objects.begin()+idx);
    delete o;
    o = NULL;
}
