#pragma once
// glew and glfw include
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Material
{
    GLuint programShader;

    private:

    public:
        Material(){

        }
        
        Material(Material& copy){
            this->programShader = copy.programShader;
        }

        ~Material(){

        }
};
