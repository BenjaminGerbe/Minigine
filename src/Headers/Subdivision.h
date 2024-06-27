#pragma once
#include "Component.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Object.h"
#include <array>

struct Triangle_mesh;

// loop mesh structure
struct Point_mesh {
    glm::vec3 point;
    std::vector<Triangle_mesh*> triangles;
    Point_mesh(glm::vec3 _point) : point(_point) {}
};

struct Triangle_mesh {
    std::array<Point_mesh*, 3> points;
    // for kobalt 
    glm::vec3 barrycenter;
    std::vector<Triangle_mesh*> CreatedTriangles;
};



class Subdivision: public Component{
    std::string SubdivisionType[3]{"loop","kobolt","Catmull"};
    int select_subdivision = 0;

    int iteration;
    public :
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();
    RTTI_DECLARATION(Subdivision)
    Subdivision(Object* obj):Component(obj){
        headerName = "Subdivision";
        SetID();
    }

    Subdivision(const Subdivision& copy):Component(copy){
        headerName = "Subdivision";
        SetID();
    }


    virtual Subdivision* Clone(Object* obj){
        Subdivision* comp = new Subdivision(obj);
        return comp;
    }

    Subdivision(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        this->headerName = "Subdivision";
    }

    void Loop();
    void Kobalt();
    void Catmull();

    std::vector<Triangle_mesh*>  GetTriangles(Point_mesh* a,Point_mesh* b);
    std::vector<Point_mesh*> ComputeTriangleLeftRightVertex(Point_mesh* a,Point_mesh* b);
    std::vector<Point_mesh*> GetAccident(Point_mesh* a);
    std::vector<Triangle_mesh*> GetAllAdjacentTriangles(Triangle_mesh* tr);

    void CreateMesh(std::vector<Triangle_mesh*> subdivised_triangles,std::string name);
    void ProcessTrianglesCreation(Point_mesh* p1,std::vector<Triangle_mesh*> accTr,Triangle_mesh* Current, std::vector<Triangle_mesh*> triangles,std::vector<Point_mesh*>& points,std::vector<Triangle_mesh*>& subdivised_triangles,bool inverse);

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);
    }

    virtual void SetID(){
        ID = c_Mesh_subdivision;
    }

    Subdivision operator=(const Subdivision& copy){
        return *this;

    }
};