// ApplicationState will store some application data, like width and height window or the OpenGLContext
// and setup the opengl context and the glfw window

#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <iostream>

class ApplicationState{
    GLFWwindow* window;
    int height;
    int width;
    bool lockMouse;

   public:

    ApplicationState(){
        lockMouse = false;
    }

    ApplicationState(int _width,int _height):width(_width),height(_height){
    }

    ~ApplicationState(){
        glfwTerminate();
    }

    bool GetLockMouse(){
        return this->lockMouse;
    }

    int SetupApplication();
    void ApplicationEvents();

    GLFWwindow* GetGLFWwindow(){
        return this->window;
    };

    void LockMouse(bool b);

    int GetHeight(){
        return this->height;
    }

    int GetWidth(){
        return this->width;
    }

    void SetWidth(int w){
        width = w;
    }

    void SetHeight(int h){
        height = h;
    }

    void SetGLFWwindow(GLFWwindow* window){
        this->window = window;
    }

};
