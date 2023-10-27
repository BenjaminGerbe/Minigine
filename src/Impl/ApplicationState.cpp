#include "../Headers/ApplicationState.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stbload/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stbload/stb_image_write.h"

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    ApplicationState* AppState = static_cast<ApplicationState*>(glfwGetWindowUserPointer(window));
     if (AppState) {
        AppState->SetHeight(height);
        AppState->SetWidth(width);
      
    }
}
    
void processInput(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
       // lockMouse = true;
    }
}

void ApplicationState::LockMouse(bool b){
  //  glfwSetInputMode(window, GLFW_CURSOR,  GLFW_CURSOR_HIDDEN);
    lockMouse = b;

    if(lockMouse){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else{
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

int ApplicationState::SetupApplication(){

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width,height,"Minigine",NULL,NULL);

        lastTime = glfwGetTime();
        nbFrames = 0;
        
        if(window == NULL){
            std::cout << "Failed to create GLFW Window" << std::endl;
            glfwTerminate();
            return 1;
        }

        glfwSetWindowUserPointer(window, this);
        glfwMakeContextCurrent(window);
        GLenum err = glewInit();

        if(GLEW_OK != err){
            std::cout << "Failed to load glew" << std::endl;
            return 1;
        }

        
        // callback for resizing window
        int w;
        int h;
        int c;
        glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

        GLFWimage images;
        images.pixels = stbi_load("Minigine.png ", &w, &h, &c, 0);;
        images.width = w;
        images.height = h;

        glfwSetWindowIcon(window, 1, &images);

        return 0;
}

static void cursor_position_callback(GLFWwindow *window, double x, double y)
{
    ApplicationState* AppState = static_cast<ApplicationState*>(glfwGetWindowUserPointer(window));
    int Height = 0;
    int Width = 0;
    if (AppState) {
        Height = AppState->GetHeight();
        Width = AppState->GetWidth();
    }   


    ImGuiIO& io = ImGui::GetIO();
    io.MousePos.x = x;
    io.MousePos.y = y;
}

void ApplicationState::ApplicationEvents(){
    processInput(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
  
     //  ImGui::SetMouseCursor(ImGuiMouseCursor_None); 
    glfwSetCursorPosCallback(window, cursor_position_callback); 

    
}