#pragma once
#include "Component.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Object.h"
    
struct Edge;
struct Face;
struct Tetrahede;

enum Visible{
    none,
    border,
    see,
    unsee
};

struct Vertex{
    glm::vec3 position;
    Visible visible;
    std::vector<Edge*> edges;
};

struct Edge{
    Vertex* v1;
    Vertex* v2;
    Visible visible;
    std::vector<Face*> faces;

    
    Vertex* operator[](int i){
        if (i == 0){
            return v1;
        }
        else if(i == 1){
            return v2;
        }

        return v1;
    }


};
struct Face{
    Edge* e1;
    Edge* e2;
    Edge* e3;
    Visible visible;
    std::vector<Tetrahede*>  lstTetrahede;
    glm::vec3 centerPosition;

     Edge* operator[](int i){
        if (i == 0){
            return e1;
        }
        else if(i == 1){
            return e2;
        }
        else if(i == 2){
            return e3;
        }

        return nullptr;
     }

};

struct Tetrahede{
    Face* f1;
    Face* f2;
    Face* f3;
    Face* f4;

    Face* operator[](int i){
        if(i == 0){
            return f1;
        }
        else if(i == 1){
            return f2;
        }
        else if(i == 2){
            return f3;
        }
        else {
            return f4;
        }
    }

    void SetFace(int i,Face* f){
        if(i == 0){
            f1 = f;
        }
        else if(i == 1){
            f2 = f;
        }
        else if(i == 2){
            f3 = f;
        }
        else{
            f4 = f;
        }
    }
};


class LineRenderer3D: public Component{

    float maxRadius;
    int nombrePoint;
    Projet* projet;
    std::vector<Object*> lstObject;
    std::vector<Object*> lstPolygone;
    std::vector<Face*> faces;
    std::vector<Edge*> edges;
    std::vector<Vertex*> vertices;
    std::vector<Tetrahede*> tetrahedes;
    bool update;
    int step = 0;
    int stepTriangulation = 0;
    int laststep;
    int laststepTriangulation;
    int lastIdx;
    int lastIdxTriangulation;

    public :
    RTTI_DECLARATION(LineRenderer3D)
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();

    LineRenderer3D(Object* obj):Component(obj){
        headerName = "LineRenderer3D";
        
        SetID();
    }

    LineRenderer3D(const LineRenderer3D& copy):Component(copy){
        headerName = "LineRenderer3D";
        SetID();
    }

    virtual LineRenderer3D* Clone(Object* obj){
        LineRenderer3D* comp = new LineRenderer3D(obj);
        return comp;
    }



    LineRenderer3D(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        this->headerName = "LineRenderer3D";
    }

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);
    }

    virtual void SetID(){
        ID = c_LineRenderer3D;
    }
    void GeneratePoint();
    void CreateBone();
    void CreateConvexShape();
    void TriangulationDelaunay();
    LineRenderer3D operator=(const LineRenderer3D& copy){
        return *this;
    }
};