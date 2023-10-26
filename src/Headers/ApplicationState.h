// ApplicationState will store some application data, like width and height window or the OpenGLContext
// and setup the opengl context and the glfw window

#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <iostream>
// glm include
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>


class ApplicationState{
    GLFWwindow* window;
    int height;
    int width;
    bool lockMouse;
    double lastTime;
    int nbFrames;
    glm::vec2 MousePosition;
    glm::mat4* MVP;

   public:

    ApplicationState(){
        lockMouse = false;
        MVP = nullptr;
    }
    
    ApplicationState(int _width,int _height):width(_width),height(_height){
        lockMouse = false;
        MVP = nullptr;
      
    }

    ~ApplicationState(){
        glfwTerminate();
    }

    glm::vec2 GetMousePosition(){
        return MousePosition;
    }

    glm::mat4* GetMVP(){
        return MVP;
    }

    void SetMVP(glm::mat4* MVP){
        this->MVP = MVP;
    }

    void SetMousePosition(glm::vec2 MousePosition){
        this->MousePosition = MousePosition;
    }

    bool GetLockMouse(){
        return this->lockMouse;
    }

    int GetFrames(){
        return nbFrames;
    }

    void SetFrames(int f){
        nbFrames = f;
    }

    double GetLastTime(){
        return lastTime;
    }

    void SetLastTime(double d){
        lastTime =d;
    }

    double GetGlfTime(){
        return glfwGetTime();
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
