#include "../Headers/RenderContext.h"


// Shaded ============
unsigned int RenderContextShaded::RenderScene(glm::mat4* MVP,Scene* scene){
    
 
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    glViewport(0,0,(int)width,(int)height);
    glClearColor(0.23f,0.23f,0.23f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    scene->Render(MVP,0);
    
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    return Tex;
};

// WireFrame
unsigned int RenderContextWireFrame::RenderScene(glm::mat4* MVP,Scene* scene){
    
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    glViewport(0,0,(int)width,(int)height);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    scene->Render(MVP,1);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    return Tex;
};


// ShadedWireFrame
unsigned int RenderContextShadedWireFrame::RenderScene(glm::mat4* MVP,Scene* scene ){

    glBindFramebuffer(GL_FRAMEBUFFER,FBO);

    glViewport(0,0,(int)width,(int)height);
    glClearColor(0.23f,0.23f,0.23f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    scene->Render(MVP,0);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    scene->Render(MVP,1);


    glBindFramebuffer(GL_FRAMEBUFFER,0);

    return Tex;
};


// NO frame
unsigned int RenderContextNOFramed::RenderScene(glm::mat4* MVP, Scene* scene){

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    scene->Render(MVP,0);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    scene->Render(MVP,1);
  

    return 0;
};

void RenderContextNOFramed::SetUp(int width,int height ){
    this->width = width;
    this->height = height;

 
};

// RenderContext =========
unsigned int RenderContext::RenderScene(glm::mat4* MVP,Scene* scene){
    std::cout << "THe RenderContext scene must not be call, instead instantiate his child and call RenderScene" << std::endl;
    assert(false);
    return 0;
}

void RenderContext::SetUp(int width,int height){
    this->width = width;
    this->height = height;

    glGenTextures(1,&Tex);
    glBindTexture(GL_TEXTURE_2D,Tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    glGenFramebuffers(1,&FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,Tex,0);
    
    glGenRenderbuffers(1,&RBO);
    glBindRenderbuffer(GL_RENDERBUFFER,RBO);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,width,height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,RBO);

    
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
    
void RenderContext::UpdateRender(int width,int height){
   this->width = width;
   this->height = height;

    glDeleteTextures(1,&Tex);
    glGenTextures(1,&Tex);
    glBindTexture(GL_TEXTURE_2D,Tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glDeleteFramebuffers(1,&FBO);
    glGenFramebuffers(1,&FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,Tex,0);
    
    glDeleteRenderbuffers(1,&RBO);
    glGenRenderbuffers(1,&RBO);
    glBindRenderbuffer(GL_RENDERBUFFER,RBO);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,width,height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,RBO);

    
    glBindFramebuffer(GL_FRAMEBUFFER,0);

   // MVP[1] = glm::perspectiveFov(glm::radians(45.0f), (float)width, (float)height, 0.5f, 1000.0f);
}

int RenderContext::GetHeight(){
    return height;
}

int RenderContext::GetWidth(){
    return width;
}

