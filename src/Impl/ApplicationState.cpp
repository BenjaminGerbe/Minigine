#include "../Headers/ApplicationState.h"

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    ApplicationState* AppState = static_cast<ApplicationState*>(glfwGetWindowUserPointer(window));
     if (AppState) {
        AppState->SetHeight(height);
        AppState->SetWidth(width);
      
    }
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
}

int ApplicationState::SetupApplication(){

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width,height,"Minigine",NULL,NULL);

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
        glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

        return 0;
}

void ApplicationState::ApplicationEvents(){
    processInput(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
}