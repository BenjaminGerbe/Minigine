#pragma once
#include "GL/glew.h"
#include "GLShader.h"

class Shader
{
    GLuint shaderIDX;

    public:
    Shader(char* vs,char* frag){
        GLShader shader;
        shader.LoadVertexShader(vs);
        shader.LoadFragmentShader(frag);
        shader.Create();

        shaderIDX = shader.GetProgram();
    }

    ~Shader(){

    }

    GLuint GetIDX(){
        return this->shaderIDX;
    }
};

