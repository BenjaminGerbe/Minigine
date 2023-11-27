#pragma once
#include "GL/glew.h"
#include "GLShader.h"

class Shader
{
    GLuint shaderIDX;
    GLuint ShaderWireFrame;
    char* vs;
    char* frag;
    public:
    Shader(char* vs,char* frag){
        GLShader shader;
        shader.LoadVertexShader(vs);
        shader.LoadFragmentShader(frag);
        shader.Create();
        this->vs = vs;
        this->frag = frag;
        shaderIDX = shader.GetProgram();

        shader.LoadVertexShader(vs);
        shader.LoadFragmentShader("FragWireFrame.glsl");
        shader.Create();
        ShaderWireFrame = shader.GetProgram();
    }

    void Compile(){
        GLShader shader;
        shader.LoadVertexShader(vs);
        shader.LoadFragmentShader(frag);
        shader.Create();

        shaderIDX = shader.GetProgram();
        shader.LoadVertexShader(vs);
        shader.LoadFragmentShader("FragWireFrame.glsl");
        shader.Create();
        ShaderWireFrame = shader.GetProgram();
    }

    ~Shader(){
   
    }

    GLuint GetIDX(){
        return this->shaderIDX;
    }

    GLuint GetWireFrame(){
        return this->ShaderWireFrame;
    }
};

