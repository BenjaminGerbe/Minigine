#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;
};

class Mesh{
    unsigned int VAO;
    unsigned int* indices;
    float* vertices;
    float* uv;
    int lenV;
    int lenI;
    char* name;
    uint32_t ID;

    public :

    Mesh(float* _vertices,int lenV,unsigned int* _indices,int lenI,int stride,char* _name):indices(_indices),vertices(_vertices),name(_name){
        unsigned int VBO,EBO;
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);
        glGenVertexArrays(1, &VAO);  
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,lenV,_vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,lenI,_indices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT)*stride,(void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        ID = std::hash<std::string>()(name);

        this->lenV = lenV;
        this->lenI = lenI;
    }

    int TriangleToDraw(){
        return lenI;
    }

    char* GetName(){
        return this->name;
    }

    uint32_t GetID(){
        return this->ID;
    }

    unsigned int GetVAO(){
        return VAO;
    };
};

