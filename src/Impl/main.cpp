
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
#include "implot.h"

// my Headers include

#include "../Headers/GLShader.h"
#include "../Headers/ApplicationState.h"
#include "../Headers/Scene.h"
#include "../Headers/RenderContext.h"
#include "../Headers/RenderContextDisplay.h"
#include "../Headers/DisplayerManager.h"
#include "../Headers/Saver.h"
#include "../Headers/Projet.h"
#include "../Headers/Shader.h"
#include "../Headers/Material.h"
#include "../data.h"
#include <new>
#define TINYOBJLOADER_IMPLEMENTATION 
#include "../tinyobj/tiny_obj_loader.h"

#include "../stbload/stb_image.h"
#include "../stbload/stb_image_write.h"




static ApplicationState app; 
Scene* scene;

DisplayerManager displayerManager;
GameView gameView;
#include "TCHAR.h"
#include "pdh.h"

struct ObjState {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

ObjState* LoadObj(const std::string& filePath) {
    ObjState* objState = new ObjState();

    tinyobj::ObjReaderConfig reader_config;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filePath, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "Error loading obj file: " << reader.Error();
        }
        return objState; // Return an empty state if loading fails
    }

    if (!reader.Warning().empty()) {
        std::cerr << "Warning loading obj file: " << reader.Warning();
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            objState->vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            objState->vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            objState->vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if (index.normal_index >= 0) {
                objState->vertices.push_back( attrib.normals[3*size_t(index.normal_index)+0]);
                objState->vertices.push_back( attrib.normals[3*size_t(index.normal_index)+1]);
                objState->vertices.push_back( attrib.normals[3*size_t(index.normal_index)+2]);
            }

            objState->indices.push_back(static_cast<unsigned int>(objState->indices.size()));
        }
    }

    return objState;
}
static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

void LoadProjetImage(char* path,Projet& projet){
    int w,h,c;
    GLuint texID;
    unsigned char* data = stbi_load(path,&w,&h,&c,0);

    GLenum v = GL_RGBA;
    if(c <= 3){
        v = GL_RGB;
    }

    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_2D,texID);
    glTexImage2D(GL_TEXTURE_2D,0,v,w,h,0,v,GL_UNSIGNED_BYTE,data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    projet.AddTexID(texID);
    glBindTexture(GL_TEXTURE_2D,0);
}



int main(int, char**){

    std::cout << "Launch Minigine" << std::endl; 
    Object::CreatePool(1000);
    

    scene = new Scene();
   // Application Setup
    app = ApplicationState((int)(1920),(int)(1080));
    int err = app.SetupApplication();    

    Projet& projet = *(new Projet());
    projet.AddScene(scene);
    projet.SetAppState(&app);

    if(err == 1){
        return 1;
    }

    
    //ObjState* obj = LoadObj("WaterSurfaceobj.obj");
    ObjState* obj = LoadObj("Plane.obj");
    ObjState* icoObj = LoadObj("Ico.obj");

    // Create Shader/Material
    Shader* shader = new Shader("frag.glsl","vs.glsl");
    Shader* pbrShader = new Shader("PBR.vs.glsl","PBR.fs.glsl");
    Shader* waterShader = new Shader("Watervs2.glsl","Waterfs.glsl");
    Material* mat = new Material(shader,"Default Material");
    PBRMaterial* green = new PBRMaterial(pbrShader,"Green");
    PBRMaterial* red = new PBRMaterial(pbrShader,"Red");
    PBRMaterial* violet = new PBRMaterial(pbrShader,"Violet");
    PBRMaterial* bleu = new PBRMaterial(pbrShader,"Bleu");
    PBRMaterial* none = new PBRMaterial(pbrShader,"None");
    WaterMaterial* mat_water = new WaterMaterial(waterShader,"Water Material");
    PBRMaterial* PBRmat = new PBRMaterial(pbrShader,"PBR Material");
    
    Saver::GenerateFactory();

    green->SetDiffuse(0.0,1.0,0.0);
    green->SetAmbiante(0.0,0.90,0.0);

    red->SetDiffuse(1.0,0.0,0.0);
    red->SetAmbiante(0.90,0.0,0.0);

    none->SetDiffuse(1.0,1.0,1.0);
    none->SetAmbiante(0.90,0.90,0.90);

    bleu->SetDiffuse(0.0,0.0,1.0);
    bleu->SetAmbiante(0.0,0.0,0.90);

    violet->SetDiffuse(1.0,0.0,1.0);
    violet->SetAmbiante(0.90,0.0,0.90);

    projet.AddMaterial(mat);
    projet.AddMaterial(PBRmat);
    projet.AddMaterial(mat_water);
    projet.AddMaterial(green);
    projet.AddMaterial(none);
    projet.AddMaterial(violet);
    projet.AddMaterial(bleu);
    projet.AddMaterial(red);

    LoadProjetImage("Minigine.png",projet);
    LoadProjetImage("Cube.png",projet);
    LoadProjetImage("Light.png",projet);
    LoadProjetImage("brickwall.jpg",projet);
    LoadProjetImage("brickwall_normal.jpg",projet);
    LoadProjetImage("board.png",projet);
    LoadProjetImage("wP.png",projet);
    LoadProjetImage("bP.png",projet);
    LoadProjetImage("wN.png",projet);
    LoadProjetImage("bN.png",projet);
    LoadProjetImage("wB.png",projet);
    LoadProjetImage("bB.png",projet);
    LoadProjetImage("wR.png",projet);
    LoadProjetImage("bR.png",projet);
    LoadProjetImage("wQ.png",projet);
    LoadProjetImage("bQ.png",projet);
    LoadProjetImage("wK.png",projet);
    LoadProjetImage("bK.png",projet);

    Mesh* m_Cube = new Mesh(verticesArray,sizeof(verticesArray),indicesArray,sizeof(indicesArray),6,"CUBE");
    Mesh* m_Dragon = new Mesh(DragonVertices,sizeof(DragonVertices),DragonIndices,sizeof(DragonIndices),8,"DRAGON");
    
    Mesh* m_Paysage = new Mesh(&obj->vertices[0],obj->vertices.size()*sizeof(float),
    &obj->indices[0],obj->indices.size()*(sizeof(unsigned int)),6,"Paysage");

    Mesh* m_Ico = new Mesh(&icoObj->vertices[0],icoObj->vertices.size()*sizeof(float),
    &icoObj->indices[0],icoObj->indices.size()*(sizeof(unsigned int)),6,"Ico");
    
    Mesh* m_Plane = new Mesh(nullptr,0.0f,nullptr,0.0f,0,"BLANK");
    Mesh* m_blank = new Mesh(nullptr,0.0f,nullptr,0.0f,0,"BLANK");

    projet.AddPrimitive(m_blank);
    projet.AddPrimitive(m_Cube);
    projet.AddPrimitive(m_Dragon);
    projet.AddPrimitive(m_Plane);
    projet.AddPrimitive(m_Paysage);
    projet.AddPrimitive(m_Ico);

    projet.SetUpDefaultObject();
    projet.SetUpComponents();

    displayerManager.AddRenderContextDisplay(new RenderContextDisplay());

    GLFWwindow* window = app.GetGLFWwindow();
    float height = (float)app.GetHeight();
    float width = (float)app.GetWidth();

    glm::mat4 matricesWire[2];
    matricesWire[0] =  glm::translate(glm::mat4(1.f), -glm::vec3({ 0.f, .6f,5.f }));
    matricesWire[0] *= glm::rotate(glm::mat4(1.0f), 15 * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0, 0.0));
    matricesWire[0] *= glm::rotate(glm::mat4(1.0f), 0.0f * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0, 0.0));
 
    scene->SetUp();
   
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);  
    glEnable(GL_BLEND);  
    glEnable(GL_MULTISAMPLE);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    Factory fac;

  

    float i = 0;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.FontGlobalScale = 1.7f;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg]               = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.41f, 0.41f, 0.41f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.41f, 0.41f, 0.41f, 0.54f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.45f, 0.45f, 0.45f, 0.54f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.21f, 0.21f, 0.21f, 0.80f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.97f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.00f, 0.00f, 0.00f, 0.97f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.21f, 0.20f, 0.21f, 0.80f);

    ImGui::GetStyle().WindowRounding = 9.0f;
    ImGui::GetStyle().FramePadding = ImVec2(11.0f,11.0f);
    ImGui::GetStyle().WindowBorderSize = 0.0f;
    ImGui::GetStyle().WindowTitleAlign = ImVec2(.5f,.5f);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    for(Object* obj : projet.GetScene()->GetObjects()){
        for(Component* c: obj->GetComponents()){
            c->SetUp();
        }
    }

    while(!glfwWindowShouldClose(window)){
        i += 0.1f;

        height = (float)app.GetHeight();
        width = (float)app.GetWidth();

        // rendering stuff
        glViewport(0,0,(int)width,(int)height);
        glClearColor(0.94f,0.91f,0.90f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Scene* sceneBuffer = projet.GetScene();
        std::vector<LightComp*> Lights = sceneBuffer->GetLightComp();
        std::vector<CameraComp*> Cameras = sceneBuffer->GetCameras();
        int idx = 0;
        for (int i = 0; i < projet.GetScene()->GetObjects().size(); i++)
        {
            Object* obj = projet.GetScene()->GetObjects()[i];
            for(Component* c: obj->GetComponents()){
                if(c->GetID() == c_Light && std::find(Lights.begin(),Lights.end(),dynamic_cast<LightComp*>(c)) == Lights.end()){
                    sceneBuffer->AddLight(dynamic_cast<LightComp*>(c));
                }
                if(c->GetID() == c_Camera && std::find(Cameras.begin(),Cameras.end(),dynamic_cast<CameraComp*>(c)) == Cameras.end()){
                    sceneBuffer->AddCamera(dynamic_cast<CameraComp*>(c));
                }
                
                c->Update();
            }
        }

        // UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        displayerManager.RenderAllRenderWindows((int)(width/2.0),(int)(height/2.0),&projet);
        displayerManager.SceneEditor(&projet);
        displayerManager.ObjectEditor(&projet);
        displayerManager.RenderGameView(&gameView,&projet);
        displayerManager.MiniMLWindows();

       // ImGui::ShowStyleEditor();
        ImGui::ShowDemoWindow();
        ImPlot::ShowDemoWindow();

        displayerManager.RenderAppOptions(&projet);
        displayerManager.MachineState(&projet);
        displayerManager.RenderSceneViewOption();
        ImGui::Render();

      
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // event
            app.ApplicationEvents();
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    return 0;
}
