
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

void ShowExampleAppDockSpace(bool* p_open)
{
    // READ THIS !!!
    // TL;DR; this demo is more complicated than what most users you would normally use.
    // If we remove all options we are showcasing, this demo would become:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    //     }
    // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
    // In this specific demo, we are not using DockSpaceOverViewport() because:
    // - (1) we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
    // - (2) we allow the host window to have padding (when opt_padding == true)
    // - (3) we expose many flags and need a way to have them visible.
    // - (4) we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport()
    //      in your code, but we don't here because we allow the window to be floating)

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        //ShowDockingDisabledMessage();
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
            if (ImGui::MenuItem("Flag: NoDockingSplit",         "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
            if (ImGui::MenuItem("Flag: NoUndocking",            "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
            if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                   { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                *p_open = false;
            ImGui::EndMenu();
        }
       
        ImGui::EndMenuBar();
    }

    ImGui::End();
}


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

    //cube1.SetTransformation(glm::translate(glm::mat4(1.f), glm::vec3({ 2.f, 1.f,0.f })));
    //cube2.SetTransformation(glm::translate(glm::mat4(1.f), glm::vec3({ -2.f, 1.f,0.f })));
    //Dragon.SetTransformation(glm::scale(glm::mat4(1.f), glm::vec3({ .2f, .2f,.2f })));

    scene.AddObjectScene(&cube1);
    scene.AddObjectScene(&cube2);
    scene.AddObjectScene(&Dragon);

    renderContextDisplay.AddRender(&rcShaded);
    renderContextDisplay.AddRender(&renderContext);
    renderContextDisplay.AddRender(&renderContextShaded);

    displayerManager.AddRenderContextDisplay(new RenderContextDisplay(renderContextDisplay));
    displayerManager.SetTemplateRenderContextDisplay(&renderContextDisplay);

    GLFWwindow* window = app.GetGLFWwindow();
    float height = (float)app.GetHeight();
    float width = (float)app.GetWidth();

    glm::mat4 matricesWire[2];
    matricesWire[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, .6f,5.f }));
    matricesWire[0] *= glm::rotate(glm::mat4(1.0f), 15 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
    matricesWire[0] *= glm::rotate(glm::mat4(1.0f), 0.0f * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0, 0.0));
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
    ImGui::GetStyle().WindowTitleAlign = ImVec2(.5f,.5f);
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

       
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        displayerManager.RenderAllRenderWindows((int)(width/2.0),(int)(height/2.0),&scene);
        displayerManager.SceneEditor(&scene);
        displayerManager.ObjectEditor(&scene);

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
