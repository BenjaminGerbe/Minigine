// ApplicationState will store some application data, like width and height window or the OpenGLContext

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class ApplicationState{
   GLFWwindow* window;
    int height;
    int width;

   public:
    ApplicationState(){
        width = 0;
        height = 0;
    }

    void SetupApplication(){

    }


    ApplicationState(int _width,int _height):height(_height),width(_width){ };
    ApplicationState(int _width,int _height,GLFWwindow* glfwindow):height(_height),width(_width),window(glfwindow){ };

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
