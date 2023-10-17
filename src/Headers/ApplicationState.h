// ApplicationState will store some application data, like width and height window or the OpenGLContext
// and setup the opengl context and the glfw window

#ifndef __ContextLibs__
#define __ContextLibs__
#define GLEW_STATIC 1
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#endif

#include <iostream>

class ApplicationState{
    GLFWwindow* window;
    int height;
    int width;

   public:

    ApplicationState() = default;

    ApplicationState(int _width,int _height):width(_width),height(_height){
    }

    ~ApplicationState(){
        glfwTerminate();
    }

    int SetupApplication();
    void ApplicationEvents();

    GLFWwindow* GetGLFWwindow(){
        return this->window;
    };

    int GetHeight(){
        return this->height;
    }

    int GetWidth(){
        return this->width;
    }

    void SetGLFWwindow(GLFWwindow* window){
        this->window = window;
    }

};
