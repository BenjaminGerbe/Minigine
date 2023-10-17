#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
class Mesh{
    unsigned int VAO;
    unsigned int* indices;
    double* vertices;
    float* uv;
    int lenV;
    int lenI;

    public :

    Mesh(double* _vertices,int lenV,unsigned int* _indices,int lenI,int stride):indices(_indices),vertices(_vertices){
        unsigned int VBO,EBO;
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);
        glGenVertexArrays(1, &VAO);  
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,lenV,_vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,lenI,_indices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_DOUBLE,GL_FALSE,stride*sizeof(double),(void*)0);
        glEnableVertexAttribArray(0);

        this->lenV = lenV;
        this->lenI = lenI;
    }

    int TriangleToDraw(){
        return lenI;
    }

    unsigned int GetVAO(){
        return VAO;
    };
};

