#include <iostream>
#define GLEW_STATIC 1
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLShader.h"
#include "../data.h"
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>
#include "ApplicationState.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ApplicationState app; 

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
   // glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
}

int main(int, char**){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    int height = 1080;
    int width = 1920;
    GLFWwindow* window = glfwCreateWindow(width,height,"LOPGL",NULL,NULL);
    

    if(window == NULL){
        std::cout << "Failted to create GLFW Window" << std::endl;
        glfwTerminate();
        return 1;
    }
   
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();

    if(GLEW_OK != err){
        std::cout << "Failted to load glew" << std::endl;
    }

    // callback for resizing window
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    // creation of the Vertex buffer object, element buffer object and le VertexArrayObject
    unsigned int VBO,EBO,VAO,UBO,FBO;

    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&UBO);
    glBindBuffer(GL_UNIFORM_BUFFER,UBO);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*2,nullptr,GL_STREAM_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER,0,UBO);

    glGenBuffers(1,&FBO);

    glEnable(GL_SCISSOR_TEST);

    // create a shader, this classse is from Bengougam, and allow me to load shaders from files, and do basic load stuff for shader
    GLShader g_basicShader;
    g_basicShader.LoadVertexShader("frag.glsl");
    g_basicShader.LoadFragmentShader("vs.glsl");
    g_basicShader.Create();

    uint32_t programShader = g_basicShader.GetProgram();
    float i = 0;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.64f, 0.68f, 0.82f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.64f, 0.68f, 0.82f, 1.00f);
    ImGui::GetStyle().WindowRounding = 9.0f;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while(!glfwWindowShouldClose(window)){
        i += 0.1f;
        processInput(window);

        // UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    
        ImGui::NewFrame();
    
        //ImGui::ShowDemoWindow();
        ImGui::SetNextWindowPos(ImVec2(0,0),1,ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(width/5.0f,height));
        int WindowsFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

        ImGui::Begin("Tweeks Window",false,WindowsFlags);
        ImGui::End();
       
        ImGui::Render();

      

        // rendering stuff
        glViewport(0,0,width/2,height);
        glScissor(0,0,width/2,height);
        glClearColor(0.23f,0.23f,0.23f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programShader);
        
        glm::vec3 cameraPosition = glm::vec3({ 0.f, 0.f,2.f });
        glm::mat4 cameraView = glm::translate(glm::mat4(1.f), -cameraPosition);
         
        cameraView *= glm::rotate(glm::mat4(1.0f), i * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0, 0.0));
        cameraView *= glm::rotate(glm::mat4(1.0f), 2 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
        
        glm::mat4 matrices[2];
        matrices[0] =  cameraView;
        matrices[1] =  glm::perspectiveFov(glm::radians(90.0f), (float)width/2.0f, (float)height, 0.1f, 1000.0f);
       
        glBindBuffer(GL_UNIFORM_BUFFER,UBO);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*2,matrices,GL_STREAM_DRAW);
        
        glDrawElements(GL_TRIANGLES,21,GL_UNSIGNED_INT,(void*)0);

        
        glViewport(width/2,0,width/2,height);
        glScissor(width/2,0,width/2,height);
        glClearColor(0.23f,0.23f,0.25f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(programShader);
        
        glm::mat4 matricesOrtho[2];
        matricesOrtho[0] =  cameraView;
        float ratio = ((float)width/2.0f)/(float)height;
        matricesOrtho[1] =  glm::ortho<float>(-2*ratio, 2, -2*ratio, 2, .01f, 5.0f);
       
        glBindBuffer(GL_UNIFORM_BUFFER,UBO);
        glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*2,matricesOrtho,GL_STREAM_DRAW);
        
        glDrawElements(GL_TRIANGLES,21,GL_UNSIGNED_INT,(void*)0);
        

        
          ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // event
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
