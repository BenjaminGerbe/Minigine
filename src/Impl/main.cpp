
#include <iostream>

// glew and glfw include
#include "GL/glew.h"
#include "GLFW/glfw3.h"

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
#include "../Headers/RenderContextDisplay.h"
#include "../Headers/DisplayerManager.h"
#include "../data.h"


#define STB_IMAGE_IMPLEMENTATION
#include "../stbload/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stbload/stb_image_write.h"

static ApplicationState app; 
Scene1 scene;
RenderContextShadedWireFrame renderContext;
RenderContextShaded rcShaded;
RenderContextWireFrame renderContextShaded;
RenderContextNOFramed Noframed;
RenderContextDisplay renderContextDisplay;
DisplayerManager displayerManager;




bool p_open = true;
int main(int, char**){

    std::cout << "Launch Minigine" << std::endl; 
   // Application Setup
    app = ApplicationState((int)(1920*1.5f),(int)(1080*1.5f));
    int err = app.SetupApplication();    

  

    if(err == 1){
        return 1;
    }

    Mesh* m_Cube = new Mesh(verticesArray,sizeof(verticesArray),indicesArray,sizeof(indicesArray),3);
    Mesh* m_Dragon = new Mesh(DragonVertices,sizeof(DragonVertices),DragonIndices,sizeof(DragonIndices),8);
    Object cube1(m_Cube,"cube1");
    Object cube2(m_Cube,"cube2");
    Object Dragon(m_Dragon,"dragon");
    
    cube1.SetTransformation(glm::translate(glm::mat4(1.f), glm::vec3({ 2.f, 1.f,0.f })));
    cube2.SetTransformation(glm::translate(glm::mat4(1.f), glm::vec3({ -2.f, 1.f,0.f })));
    Dragon.SetTransformation(glm::scale(glm::mat4(1.f), glm::vec3({ .2f, .2f,.2f })));

    scene.AddObjectScene(cube1);
    scene.AddObjectScene(cube2);
    scene.AddObjectScene(Dragon);

    renderContextDisplay.AddRender(&rcShaded);
    renderContextDisplay.AddRender(&renderContext);
    renderContextDisplay.AddRender(&renderContextShaded);

    displayerManager.AddRenderContextDisplay(new RenderContextDisplay(renderContextDisplay));
    displayerManager.SetTemplateRenderContextDisplay(&renderContextDisplay);

    GLFWwindow* window = app.GetGLFWwindow();
    float height = (float)app.GetHeight();
    float width = (float)app.GetWidth();

    glm::mat4 matricesWire[2];
    matricesWire[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, 0.f,3.f }));
    matricesWire[1] =  glm::perspectiveFov(glm::radians(45.0f), (float)width, (float)height, 0.5f, 1000.0f);

    renderContext.SetUp(matricesWire,(int)(width/2.0f),(int)(height/2.0f));
    renderContextShaded.SetUp(matricesWire,(int)(width/2.0f),(int)(height/2.0f));
    rcShaded.SetUp(matricesWire,(int)(width/2.0f),(int)(height/2.0f));
    Noframed.SetUp(matricesWire,(int)(width/2.0f),(int)(height/2.0f));
    scene.SetUp((int)width,(int)height);

    rcShaded.SetLabel("Shaded");
    renderContext.SetLabel("ShadedWireFrame");
    renderContextShaded.SetLabel("WireFrame");
   
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);  

    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    
    float i = 0;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.FontGlobalScale = 1.7f;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui::StyleColorsLight();
    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_TitleBgActive]          = ImVec4(1.00f, 0.79f, 0.80f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.83f, 0.89f, 0.83f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.88f, 0.93f, 1.00f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(1.00f, 0.79f, 0.80f, 1.00f);


    ImGui::GetStyle().WindowRounding = 9.0f;
    ImGui::GetStyle().FramePadding = ImVec2(11.0f,11.0f);
    ImGui::GetStyle().WindowBorderSize = 0.0f;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");


    while(!glfwWindowShouldClose(window)){
        i += 0.1f;


        height = (float)app.GetHeight();
        width = (float)app.GetWidth();

        // rendering stuff
      glViewport(0,0,(int)width,(int)height);
        glClearColor(0.94f,0.91f,0.90f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        matricesWire[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, .6f,5.f }));
        matricesWire[0] *= glm::rotate(glm::mat4(1.0f), 15 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
        matricesWire[0] *= glm::rotate(glm::mat4(1.0f), i * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0, 0.0));
    

        // UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        
        ImGui::NewFrame();

        displayerManager.RenderAllRenderWindows((int)(width/2.0),(int)(height/2.0),&scene);
        displayerManager.SceneEditor(&scene);

        ImGui::ShowStyleEditor();
        ImGui::ShowDemoWindow();
        

        displayerManager.RenderAppOptions();
        displayerManager.MachineState();

        ImGui::Render();

      
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // event
        app.ApplicationEvents();
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
