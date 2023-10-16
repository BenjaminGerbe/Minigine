
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


#define STB_IMAGE_IMPLEMENTATION
#include "../stbload/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stbload/stb_image_write.h"

ApplicationState app; 
Scene1 scene;
RenderContextShadedWireFrame renderContext;
RenderContextShaded rcShaded;
RenderContextWireFrame renderContextShaded;
RenderContextNOFramed Noframed;

int main(int, char**){

    std::cout << "Launch Minigine" << std::endl; 
   // Application Setup
    app = ApplicationState(1920,1080);
    int err = app.SetupApplication();    

    if(err == 1){
        return 1;
    }

    GLFWwindow* window = app.GetGLFWwindow();
    float height = (float)app.GetHeight();
    float width = (float)app.GetWidth();

    glm::mat4 matrices[2];
    matrices[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, 0.f,3.f }));
    matrices[1] =   glm::perspectiveFov(glm::radians(45.0f), (float)width, (float)height, 0.5f, 1000.0f);

    glm::mat4 matricesWire[2];
    matricesWire[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, 0.f,3.f }));
    matricesWire[1] =  glm::perspectiveFov(glm::radians(45.0f), (float)width, (float)height, 0.5f, 1000.0f);

    renderContext.SetUp(matricesWire,(int)(width/2.0f),(int)(height/2.0f));
    renderContextShaded.SetUp(matrices,(int)(width/2.0f),(int)(height/2.0f));
    rcShaded.SetUp(matricesWire,(int)(width/2.0f),(int)(height/2.0f));
    Noframed.SetUp(matricesWire,(int)(width/2.0f),(int)(height/2.0f));
    scene.SetUp((int)width,(int)height);

   
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);  
    glCullFace(GL_FRONT); 
    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    
    float i = 0;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.FontGlobalScale = 1.3f;
    ImGui::StyleColorsLight();
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_TitleBg]                = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(1.00f, 0.79f, 0.80f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.83f, 0.89f, 0.83f, 1.00f);


    ImGui::GetStyle().WindowRounding = 9.0f;
    ImGui::GetStyle().FramePadding = ImVec2(11.0f,11.0f);
    ImGui::GetStyle().WindowBorderSize = 0.0f;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    while(!glfwWindowShouldClose(window)){
        i += 0.1f;

        // rendering stuff
        glViewport(0,0,(int)width,(int)height);
        glClearColor(0.98f,0.95f,0.94f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       // Noframed.RenderScene(&scene);

        matrices[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, 0.f,10.f }));
        matrices[0] *= glm::rotate(glm::mat4(1.0f), 80 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
        matrices[0] *= glm::rotate(glm::mat4(1.0f), i * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0, 0.0));

        matricesWire[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, 0.f,7.f }));
        matricesWire[0] *= glm::rotate(glm::mat4(1.0f), 50 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
        matricesWire[0] *= glm::rotate(glm::mat4(1.0f), i * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0, 0.0));
    

        unsigned int tex2 = renderContext.RenderScene(&scene);
        unsigned int tex = renderContextShaded.RenderScene(&scene);
        unsigned int tex3 = rcShaded.RenderScene(&scene);

        // UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        
        ImGui::NewFrame();

        ImGui::ShowStyleEditor();

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + 0));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 3));
       
       ImGuiWindowFlags window_flags = 0
		| ImGuiWindowFlags_NoTitleBar 
		| ImGuiWindowFlags_NoResize 
		| ImGuiWindowFlags_NoMove 
		| ImGuiWindowFlags_NoScrollbar 
		| ImGuiWindowFlags_NoSavedSettings
		;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f,5.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(0.89f, 0.94f, 0.97f, 1.00f));

	    ImGui::Begin("TOOLBAR", NULL, window_flags);
	    ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(1);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,6);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(3.0f,3.0f));
        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.9f, 0.94f, 0.97f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(0.9f, 0.94f, 0.97f, 1.00f));
        static bool pressed;

        if (ImGui::BeginMenuBar())
        {
             if (ImGui::BeginMenu("Menu"))
            {
                ImGui::MenuItem("Main menu bar");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::ShowDemoWindow();

        if(!ImGui::IsItemHovered() && pressed){
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::BeginPopupModal("FileOptions")) {
            // Draw popup contents.
            ImGui::EndPopup();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::GetStyle().WindowRounding = 6.0f;
	    ImGui::End();

       
         {
            ImGui::SetNextWindowSize(ImVec2((float)renderContext.GetWidth(),(float)renderContext.GetHeight()));
            ImGui::Begin("Wireframe game view");

                ImGui::BeginChild("Game Render");

                    ImVec2 size = ImGui::GetWindowSize();

                   ImGui::Image((void*)(intptr_t)tex,size,ImVec2(0,1 ),ImVec2(1,0 ));

                ImGui::EndChild();


            ImGui::End();
        }

        {
            ImGui::SetNextWindowSize(ImVec2((float)renderContextShaded.GetWidth(),(float)renderContextShaded.GetHeight()));
            ImGui::Begin("Shaded Wireframe game view ");

                ImGui::BeginChild("Game Render");

                    ImVec2 size = ImGui::GetWindowSize();

                   ImGui::Image((void*)(intptr_t)tex2,size,ImVec2(0,1 ),ImVec2(1,0 ));

                ImGui::EndChild();


            ImGui::End();
        }

        {
            ImGui::SetNextWindowSize(ImVec2((float)rcShaded.GetWidth(),(float)rcShaded.GetHeight()));
            ImGui::Begin("Shaded game view");

                ImGui::BeginChild("Game Render");

                    ImVec2 size = ImGui::GetWindowSize();

                   ImGui::Image((void*)(intptr_t)tex3,size,ImVec2(0,1 ),ImVec2(1,0 ));

                ImGui::EndChild();


            ImGui::End();
        }


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
