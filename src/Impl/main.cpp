
#include <iostream>

// glew and glfw include
#ifndef __ContextLibs__
#define __ContextLibs__
#define GLEW_STATIC 1
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#endif

// glm include
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>

// imgui include
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// my Headers include
#include "../Headers/GLShader.h"
#include "../Headers/ApplicationState.h"
#include "../Headers/Scene.h"
#include "../Headers/RenderContext.h"

ApplicationState app; 
Scene1 scene;
RenderContextShaded renderContext;
int main(int, char**){
   
   // Application Setup
    app = ApplicationState();
    int err = app.SetupApplication();    

    if(err == 1){
        return 1;
    }

    GLFWwindow* window = app.GetGLFWwindow();
    float height = (float)app.GetHeight();
    float width = (float)app.GetWidth();


    glm::mat4 matrices[2];
    matrices[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, 0.f,3.f }));
    matrices[1] =  glm::perspectiveFov(glm::radians(90.0f), (float)width, (float)height, 0.1f, 1000.0f);

    renderContext.SetUp(matrices);
    scene.SetUp((int)width,(int)height);
   
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
        glViewport(0,0,(int)width,(int)height);
        glScissor(0,0,(int)width,(int)height);
        glClearColor(0.23f,0.23f,0.23f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        matrices[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, 0.f,30.f }));
        matrices[0] *= glm::rotate(glm::mat4(1.0f), 90 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
        matrices[0] *= glm::rotate(glm::mat4(1.0f), i * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0, 0.0));

        renderContext.RenderScene(&scene);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // event
        app.ApplicationEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
