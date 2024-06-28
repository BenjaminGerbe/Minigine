#pragma once
#include "Component.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Object.h"
#include <array>

struct Triangle_mesh;
struct Point_mesh;

typedef std::shared_ptr<Triangle_mesh> Triangle_ptr;
typedef std::shared_ptr<Point_mesh> Point_ptr;



// loop mesh structure
struct Point_mesh {
    glm::vec3 point;
    std::vector<Triangle_ptr> triangles;
    Point_mesh(glm::vec3 _point) : point(_point) {}
};

struct Triangle_mesh {
    std::array<Point_ptr, 3> points;
    // for kobalt 
    glm::vec3 barrycenter;
    std::vector<Triangle_ptr> CreatedTriangles;
};

class Subdivision: public Component{
    std::string SubdivisionType[4]{"loop","kobolt","Catmull","ButterFly"};
    int select_subdivision = 0;
    bool smooth= false;
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
    void ButterFly();

    std::vector<Triangle_ptr>  GetTriangles(Point_ptr a,Point_ptr b);
    std::vector<Point_ptr> ComputeTriangleLeftRightVertex(Point_ptr a,Point_ptr b);
    std::vector<Point_ptr> GetAccident(Point_ptr a);
    std::vector<Triangle_ptr> GetAllAdjacentTriangles(Triangle_ptr tr);
    Point_ptr Subdivision::GetTriangleAccident(Point_ptr a,Point_ptr b,Triangle_ptr triangle);

    void CreateMesh(std::vector<Triangle_ptr> subdivised_triangles,std::string name);
    void ProcessTrianglesCreation(Point_ptr p1,std::vector<Triangle_ptr> accTr,Triangle_ptr Current, std::vector<Triangle_ptr> triangles,std::vector<Point_ptr>& points,std::vector<Triangle_ptr>& subdivised_triangles,bool inverse);

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