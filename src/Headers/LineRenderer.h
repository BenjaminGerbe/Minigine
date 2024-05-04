#pragma once
#include "Component.h"
#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective
#include <ext/scalar_constants.hpp> // glm::pi
#include <gtc/type_ptr.hpp>
#include <vector>
#include <math.h> 
#include <limits>
#include <chrono>
#include <iostream>

struct PolyPoint{
    int idx;
    float angle;
    float length;
    glm::vec3 position;
};

struct Segment{
    glm::vec3 A;
    glm::vec3 B;
    bool visible;
    Segment(){
        visible = true;
    }
};

struct Triangle{
    Segment* lst[3];
    glm::vec3 circumcircle;

    Segment* operator[](int i){
        return lst[i];
    }

    Segment* GetSegment(int i){
        return lst[i];
    }

    void SetSegment(int i,Segment* s){
        lst[i] = s;
    }

    void operator()(int i,Segment* s){
        lst[i] = s;
    }

};

struct TrianglePoint{

    glm::vec3 s1;
    glm::vec3 s2;
    glm::vec3 s3;

    TrianglePoint(){
        s1 = glm::vec3(NULL);
        s2 = glm::vec3(NULL);
        s2 = glm::vec3(NULL);
    }

    glm::vec3 operator[](int i){
        
        int idx = i%3;

        if(idx == 0) return s1;
        if(idx == 1) return s2;
        if(idx == 2) return s3;
        
        return s3;
    }

    
    void setValue(int i, glm::vec3 value){
        
        int idx = i%3;
        if(idx == 0) s1 = value;
        if(idx == 1) s2 = value;
        if(idx == 2) s3 = value;
    }

    bool isComplete(){
        bool complete = true;
        for (int i = 0; i < 3; i++)
        {
            if((*this)[i].x == NULL){
                complete = false;
                break;
            }
        }
        
        return complete;
    }


    bool operator==(TrianglePoint &trianglePoint){
        float epsilon = std::numeric_limits<float>::epsilon();
        
        bool same = false;;

        for (int i = 0; i < 3; i++)
        {
            same = false;
            for (int j = 0; j < 3; j++)
            {
                if( glm::length((*this)[i] - trianglePoint[j]) < epsilon){
                    same = true; 
                }
            }

            if(!same){
                break;
            }
        }
        

        return same;
    }

};

class LineRenderer:public Component{
    Object* targetMouse;
    Projet* projet;
    std::vector<Object*> lstObject;
    std::vector<Object*> lstLines;
    std::vector<Object*> lstLinesVornoi;
    std::vector<Object*> SegmentTriangles;
    std::vector<Segment*> Segments;
    std::vector<Triangle*> trianglesPoints;
    int lastIdx;
    bool RenderLine;
    bool start ;
    bool Delaunay;
    bool DelaunayTriangulation;
    bool supression;
    float time;
    bool Voronoi;
    bool AnimimateVoronoi;
    public :
    RTTI_DECLARATION(LineRenderer)
    virtual void Editor();
    virtual void SetUp();
    virtual void Update();

    LineRenderer(Object* obj):Component(obj){
        headerName = "LineRenderer";
        RenderLine = false;
        SetID();
    }

    LineRenderer(const LineRenderer& copy):Component(copy){
        RenderLine = false;
    }

        
    virtual LineRenderer* Clone(Object* obj){
        LineRenderer* comp = new LineRenderer(obj);
        return comp;
    }

    LineRenderer( std::vector<Object*> point,Object* obj):Component(obj){
        headerName = "LineRenderer";
        RenderLine = false;
        SetID();

       CopyObject(point);

    }

    

    LineRenderer(std::string id,int i,YAML::Node& yamlFile,Object* obj):Component(id,i,yamlFile,obj){
        headerName = "LineRenderer";
        RenderLine = yamlFile[id][i]["RenderLine"].as<bool>();
    }
    
    void CopyObject( std::vector<Object*> point);

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        Component::Save(id,i,yamlFile);
        yamlFile[id][i]["RenderLine"] = RenderLine;
    }

    virtual void SetID(){
        ID = c_LineRenderer;
    }

    void CreateLine();

    void CreateLine(std::vector<Segment*> segment);

    void CreateLineTriangle(std::vector<Triangle*>lstTriangles);
    
    void GiftWraping();

    void GrahamScan();

    void Triangulation();

    void TriangulationDelaunay();

    void SuppressionDelaunay(glm::vec3 mousePosition);

    void RenderVoronoi();

    LineRenderer operator=(const LineRenderer& copy){
        return *this;
    }





    ~LineRenderer(){
       // delete targetMouse;
    }
};