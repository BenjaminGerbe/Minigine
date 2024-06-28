#include "../Headers/Subdivision.h"
#include "../Headers/Projet.h"
#include <limits>
RTTI_DEFINITION_BASE(Subdivision,Subdivision)


std::vector<Triangle_ptr> Subdivision::GetTriangles(Point_ptr a,Point_ptr b){
        std::vector<Triangle_ptr> result;
        int idx=0;
        for(int i = 0;i < a->triangles.size();i++){
            for(int j = 0;j< b->triangles.size();j++){
                if(a->triangles[i] == b->triangles[j] && std::find(result.begin(),result.end(),b->triangles[j]) == result.end()){
                   result.push_back(a->triangles[i]);
                }
            }
        }

        return result;
}

Point_ptr Subdivision::GetTriangleAccident(Point_ptr a,Point_ptr b,Triangle_ptr triangle){
        Triangle_ptr tr;
        std::vector<Triangle_ptr> lst = GetTriangles(a,b);
        Point_ptr result = nullptr;
        int idx=0;
        if(lst[idx] == triangle)
            idx++;

        for(int i = 0;i<lst[idx]->points.size();i++){
            if(lst[idx]->points[i] != a && lst[idx]->points[i] != b){
                result = lst[idx]->points[i];
            }
        }

       return result;
}

std::vector<Triangle_ptr> Subdivision::GetAllAdjacentTriangles(Triangle_ptr tr){
    std::vector<Triangle_ptr> triangles;
    for(int i = 0;i<tr->points.size();i++){
        int nxt = (i+1)%tr->points.size();
        Point_ptr p1 = tr->points[i];
        Point_ptr p2 = tr->points[nxt];
        std::vector<Triangle_ptr> accTrs = GetTriangles(p1,p2);
        if(accTrs.size() <= 1)
            continue;

        int idx=0;
        if(accTrs[idx] == tr) 
            idx=1;

        triangles.push_back(accTrs[idx]);
        
    }

    return triangles;
}

std::vector<Point_ptr> Subdivision::ComputeTriangleLeftRightVertex(Point_ptr a,Point_ptr b){
    std::vector<Point_ptr> result;
    std::vector<Triangle_ptr> trianglesAccident = Subdivision::GetTriangles(a,b);
    int idx =0;
    for(int k = 0;k<trianglesAccident.size();k++){
        for(int i = 0;i < trianglesAccident[k]->points.size();i++){
            Point_ptr point =trianglesAccident[k]->points[i];
            if(point != a && point != b){
                result.push_back(point);
            }
            
        }
    }
    
    return result;
}

Triangle_ptr GetPointerTriangle(std::vector<Triangle_ptr> triangles,Triangle_mesh mesh){
    Triangle_ptr result = nullptr;
    for (int i = 0; i < triangles.size(); i++)
    {
        bool find = true;
        int count=0;
        for(int j = 0;j<mesh.points.size();j++){
            if(std::find(triangles[i]->points.begin(),triangles[i]->points.end(),mesh.points[j]) != triangles[i]->points.end()){
                count++;
            }
            else{
                break;
            }
        }
       
        if(count >=3){
            result = triangles[i];
            break;
        }
    }
    return result;
}

std::vector<Point_ptr> Subdivision::GetAccident(Point_ptr a){
    std::vector<Point_ptr> result;
    for(int i = 0; i < a->triangles.size();i++){
        for(int j = 0;j < a->triangles[i]->points.size();j++){
            Point_ptr point = a->triangles[i]->points[j];
            if(point == a){
                continue;
            }
            result.push_back(point);
        }
    }

    return result;
}

Point_ptr GetPointerPoint(std::vector<Point_ptr>& points,glm::vec3 position){
    
    float epsilon = std::numeric_limits<float>::epsilon();
    Point_ptr p_triangle = nullptr;
    for(int j = 0;j < points.size();j++){
        glm::vec3 point = points[j]->point;
        if( std::abs(glm::length((point - position))) < epsilon ){
            p_triangle = points[j];
        }

    }

    if(p_triangle == nullptr){
        p_triangle = std::make_shared<Point_mesh>(position);
        points.push_back(p_triangle);
    }

    return p_triangle;
}

void Subdivision::ProcessTrianglesCreation(Point_ptr p1,std::vector<Triangle_ptr> accTr,Triangle_ptr Current, std::vector<Triangle_ptr> triangles,std::vector<Point_ptr>& points,std::vector<Triangle_ptr>& subdivised_triangles,bool inverse){
    int idx=0;

    if(accTr[idx] == Current) 
        idx=1;

    Triangle_mesh tr;
    Triangle_ptr kTr =(Triangle_ptr)(accTr[idx]);

    Point_ptr bCenter2 = GetPointerPoint(points,kTr->barrycenter);
    Point_ptr bCenter = GetPointerPoint(points,Current->barrycenter);
    if(!inverse){
        tr.points[0] = p1;
        tr.points[1] = bCenter;
        tr.points[2] = bCenter2;
    }
    else{
        tr.points[0] = p1;
        tr.points[2] = bCenter;
        tr.points[1] = bCenter2;
    }
    
    Triangle_ptr t;
    std::vector<Triangle_ptr> adjecent = GetAllAdjacentTriangles(Current);
    for(int k = 0; k < adjecent.size();k++){
        Triangle_ptr adTr = adjecent[k];
        t = GetPointerTriangle(adTr->CreatedTriangles,tr);
        if(t != nullptr){

            break;
        }
    }

    if(t == nullptr){
        t = std::make_shared<Triangle_mesh>();
        t->points[0] = tr.points[0];
        t->points[1] = tr.points[1];
        t->points[2] = tr.points[2];

        tr.points[0]->triangles.push_back(t);
        tr.points[1]->triangles.push_back(t);
        tr.points[2]->triangles.push_back(t);

        Current->CreatedTriangles.push_back(t);
        subdivised_triangles.push_back(t);
        t->barrycenter = (t->points[0]->point+t->points[1]->point+t->points[2]->point)/3.0f;
    }
    
}

void Subdivision::Loop(){
    std::vector<Triangle_ptr> triangles;
    std::vector<Point_ptr> points;
    Mesh mesh = *this->obj->GetMesh();
    unsigned int* indices = mesh.GetIndices(); 
    int size =  mesh.GetLenI()/sizeof(unsigned int);
    float epsilon = std::numeric_limits<float>::epsilon();

    // Intialize current Mesh
    for(int i = 0;i <size;i+=3){
        Triangle_ptr triangle = std::make_shared<Triangle_mesh>();
        for(int j=0;j<3;j++){
            Point_ptr p_triangle = GetPointerPoint(points,mesh.GetVerticesValue(indices[i+j]));
            triangle->points[j] = p_triangle;
            p_triangle->triangles.push_back(triangle);
            
        }
        
        triangles.push_back(triangle);
    }

    std::vector<Triangle_ptr> subdivised_triangles;
    std::vector<Point_ptr> NPoints;

    for(int it = 0;it < this->iteration;it++){
        subdivised_triangles.clear();
        NPoints.clear();
        for(int i = 0;i < triangles.size();i++){
            Triangle_ptr triangle = triangles[i];

            // Calculate differentes edge Point and vertex points
            std::vector<Point_ptr> v_edgePoint;
            std::vector<Point_ptr> v_vertexPoint;
            for(int j = 0; j < triangle->points.size();j++){
                int nxt = (j+1)%triangle->points.size();
                Point_ptr p1 = triangle->points[j];
                Point_ptr p2 = triangle->points[nxt];
                std::vector<Point_ptr> other_points= ComputeTriangleLeftRightVertex(p1,p2);

                if(other_points.size() <= 1){
                    glm::vec3 dir = (p2->point - p1->point);
                    glm::vec3 edgePosition =  p1->point*(4.0f/9.0f)+p2->point*(4.0f/9.0f)+other_points[0]->point*(1.0f/9.0f);
                    Point_ptr edgePoint = GetPointerPoint(NPoints,edgePosition);
                    v_edgePoint.push_back(edgePoint);
                }
                else{
                    glm::vec3 edgePosition = ((p1->point+p2->point)*(3.0f/8.0f))+
                    (other_points[0]->point+other_points[1]->point)*(1.0f/8.0f);
                    Point_ptr edgePoint = GetPointerPoint(NPoints,edgePosition);
                    v_edgePoint.push_back(edgePoint);
                }

                std::vector<Point_ptr> accidentPoints = GetAccident(p1);
                glm::vec3 vertexPosition = glm::vec3(0,0,0);

                if(accidentPoints.size() < 3){
                    vertexPosition = accidentPoints[0]->point*(1.0f/8.0f) + accidentPoints[1]->point*(1.0f/8.0f)+
                    p1->point*(3.0f/4.0f);
                }
                else{
                    float alpha = 3.0/16.0f;
                    float n = accidentPoints.size();
                    if(n > 3){
                        float d = (3.0f/8.0f)+((1.0f/4.0f)*(std::cosf(2.0f*glm::pi<float>()/n)));
                        alpha = (1.0f/n)*( (5.0f/8.0f)-(d*d));
                        if(smooth){
                            alpha = 3.0f/(8.0*n);
                        }
                    }
                    vertexPosition = (1.0f-(n*alpha))*p1->point;
                    for(int k=0;k<n;k++){
                        vertexPosition += alpha*accidentPoints[k]->point;
                    }
                }
                
                Point_ptr vertexPointer = GetPointerPoint(NPoints,vertexPosition);
                v_vertexPoint.push_back(vertexPointer);
            }


            for(int j = 0;j < (int)v_vertexPoint.size();j++){
                Triangle_ptr _nTr = std::make_shared<Triangle_mesh>();
                int prev = (j) < 0 ? v_vertexPoint.size()-1 : j;
                int nxt = (j+2)%v_edgePoint.size();
                _nTr->points[0] = v_vertexPoint[j];
                _nTr->points[2] = v_edgePoint[nxt];
                _nTr->points[1] = v_edgePoint[prev];

                _nTr->points[0]->triangles.push_back(_nTr);
                _nTr->points[1]->triangles.push_back(_nTr);
                _nTr->points[2]->triangles.push_back(_nTr);
                subdivised_triangles.push_back(_nTr);
            }

            Triangle_ptr nTr = std::make_shared<Triangle_mesh>();
            for(int j=0;j<3;j++){
                nTr->points[j] = v_edgePoint[j];
                nTr->points[j]->triangles.push_back(nTr);
            }
            subdivised_triangles.push_back(nTr);
        }
        triangles.clear();
        triangles = std::vector<Triangle_ptr>(subdivised_triangles);
    }

    float* vertices = new float[subdivised_triangles.size()*6*3];
    unsigned int* indices_triangles = new unsigned int[subdivised_triangles.size()*3];
    int v_idx =0;
    int i_idx =0;
    for(int i = 0;i < subdivised_triangles.size();i++){
        glm::vec3 p1 = subdivised_triangles[i]->points[0]->point;
        glm::vec3 p2 = subdivised_triangles[i]->points[1]->point;
        glm::vec3 p3 = subdivised_triangles[i]->points[2]->point;
        glm::vec3 tangent = p1-p2;
        glm::vec3 bisnormal = p3-p1;
        glm::vec3 normal = glm::cross(bisnormal,tangent);

        for(int j = 0; j < 3;j++){
            vertices[v_idx++]= (subdivised_triangles[i]->points[j]->point.x);
            vertices[v_idx++]=(subdivised_triangles[i]->points[j]->point.y);
            vertices[v_idx++]=(subdivised_triangles[i]->points[j]->point.z);
            vertices[v_idx++]=(normal.x);
            vertices[v_idx++]=(normal.y);
            vertices[v_idx++]=(normal.z);
            indices_triangles[i_idx] = i_idx;
            i_idx++;
        }
        
    }
    std::string name = mesh.GetName()+"_loop";
    if(smooth){
        name+="_smooth";
    }
    CreateMesh(subdivised_triangles,name);

    triangles.clear();
    subdivised_triangles.clear();
    points.clear();
    NPoints.clear();
}

void Subdivision::ButterFly(){
    std::vector<Triangle_ptr> triangles;
    std::vector<Point_ptr> points;
    Mesh mesh = *this->obj->GetMesh();
    unsigned int* indices = mesh.GetIndices(); 
    int size =  mesh.GetLenI()/sizeof(unsigned int);
    float epsilon = std::numeric_limits<float>::epsilon();

    // Intialize current Mesh
    for(int i = 0;i <size;i+=3){
        Triangle_ptr triangle = std::make_shared<Triangle_mesh>();
        for(int j=0;j<3;j++){
            Point_ptr p_triangle = GetPointerPoint(points,mesh.GetVerticesValue(indices[i+j]));
            triangle->points[j] = p_triangle;
            p_triangle->triangles.push_back(triangle);
            
        }
        
        triangles.push_back(triangle);
    }

    std::vector<Triangle_ptr> subdivised_triangles;
    std::vector<Point_ptr> NPoints;

    for(int it = 0;it < this->iteration;it++){
        subdivised_triangles.clear();
        NPoints.clear();
        for(int i = 0;i < triangles.size();i++){
            Triangle_ptr triangle = triangles[i];

            // Calculate differentes edge Point and vertex points
            std::vector<Point_ptr> v_edgePoint;
            std::vector<Point_ptr> v_vertexPoint;
            for(int j = 0; j < triangle->points.size();j++){
                int nxt = (j+1)%triangle->points.size();
                Point_ptr p1 = triangle->points[j];
                Point_ptr p2 = triangle->points[nxt];
                std::vector<Point_ptr> points = GetAccident(p1);
                std::vector<Point_ptr> points2 = GetAccident(p2);
                std::vector<Point_ptr> other_points= ComputeTriangleLeftRightVertex(p1,p2);
                std::vector<Triangle_ptr> triangles_0 = GetTriangles(p1,p2);
                std::vector<Triangle_ptr> triangles_1 = GetTriangles(p1,other_points[0]);
                std::vector<Triangle_ptr> triangles_2 = GetTriangles(p2,other_points[0]);
                bool swap = false;
                std::vector<Triangle_ptr> triangles_1_b = GetTriangles(p1,other_points[1%other_points.size()]);
                std::vector<Triangle_ptr> triangles_2_b = GetTriangles(p2,other_points[1%other_points.size()]);

                if(triangles_1_b.size() >= 2 && triangles_2_b.size() <= 1){
                    std::swap(triangles_2_b,triangles_1_b);
                    std::swap(p1,p2);
                    swap = true;
                } else if(triangles_1.size() <= 1 && triangles_2.size() >= 2 && other_points.size() >=2){
                    std::swap(triangles_1,triangles_1_b);
                    std::swap(triangles_2,triangles_2_b);
                    std::swap(other_points[0],other_points[1]);
                }
                else if(triangles_1.size() >= 2 && triangles_2.size() <= 1 && other_points.size() >=2){
                    std::swap(triangles_1,triangles_1_b);
                    std::swap(triangles_2,triangles_2_b);
                    std::swap(other_points[0],other_points[1]);
                    std::swap(triangles_2_b,triangles_1_b);
                    std::swap(p1,p2);
                    swap = true;
                }
                
                if(triangles_1_b.size() >= 2 && triangles_2_b.size() < 2 && triangles_1.size() >= 2 && triangles_2.size() <2){
                    std::swap(triangles_1,triangles_2);
                    std::swap(triangles_1_b,triangles_2_b);
                    std::swap(p1,p2);
                    swap = true;
                }

                if(triangles_1_b.size() >= 2 && triangles_2_b.size() >= 2 && triangles_1.size() < 2 && triangles_2.size() < 2){
                    std::swap(triangles_1_b,triangles_1);
                    std::swap(triangles_2_b,triangles_2);
                    std::swap(other_points[0],other_points[1]);
                }

                if(triangles_1.size() >= 2 && triangles_2.size() < 2 && triangles_1_b.size() < 2 && triangles_2_b.size() >= 2 ){
                    std::swap(triangles_1,triangles_2);
                    std::swap(triangles_1_b,triangles_2_b);
                }

                
                Point_ptr p4,p5,p6,p7;
                glm::vec3 edgePosition;
                Point_ptr edgePoint;
                if(other_points.size() <= 1 && (triangles_1.size() < 2 ||triangles_2.size() < 2) ){
                    edgePosition = p1->point*(1.0f/2.0f) + p2->point*(1.0f/2.0f) ;
                }
                else if(other_points.size() <=1){
                    p4 = GetTriangleAccident(p1,other_points[0],triangles_0[0]);
                    p5 = GetTriangleAccident(p2,other_points[0],triangles_0[0]);
                    edgePosition =  p1->point*(9.0f/16.0f)+p2->point*(9.0f/16.0f)+p4->point*(-1.0f/16.0f)+p5->point*(-1.0f/16.0f);
                }
                else if(triangles_1_b.size() < 2 && triangles_2_b.size() >= 2 && triangles_1.size() >= 2 && triangles_2.size() >=2){
                    p4 = GetTriangleAccident(p1,other_points[0],triangles_0[0]);
                    p5 = GetTriangleAccident(p2,other_points[0],triangles_0[0]);
                    p6 = GetTriangleAccident(p2,other_points[1],triangles_0[1]);
                    edgePosition =  p1->point*(5.0f/8.0f)+p2->point*(3.0f/8.0f)+other_points[0]->point*(3.0f/16.0f)+other_points[1]->point*(1.0f/16.0f);
                    edgePosition += p4->point*(-1.0f/8.0f)+ p5->point*(-1.0f/16.0f)+p6->point*(-1.0f/16.0f);
                }
                else if(triangles_1_b.size() < 2 && triangles_2_b.size() >= 2 && triangles_1.size() < 2 && triangles_2.size() >=2){
                    p4 = GetTriangleAccident(p2,other_points[0],triangles_0[0]);
                    p5 = GetTriangleAccident(p2,other_points[1],triangles_0[0]);
                    edgePosition =  p1->point*(3.0f/8.0f)+p2->point*(5.0f/8.0f)+other_points[0]->point*(1.0f/16.0f)+other_points[1]->point*(1.0f/16.0f);
                    edgePosition += p4->point*(-1.0f/16.0f)+ p5->point*(-1.0f/16.0f);
                }
                else if(triangles_1_b.size() < 2 && triangles_2_b.size() < 2 && triangles_1.size() >= 2 && triangles_2.size() >=2){
                    p4 = GetTriangleAccident(p1,other_points[0],triangles_0[0]);
                    p5 = GetTriangleAccident(p2,other_points[0],triangles_0[0]);
                    edgePosition =  p1->point*(1.0f/2.0f)+p2->point*(1.0f/2.0f)+other_points[0]->point*(1.0f/4.0f);
                    edgePosition += p4->point*(-1.0f/8.0f)+ p5->point*(-1.0f/8.0f);
                }
                else if(triangles_1.size() < 2 && triangles_2.size() >= 2 && triangles_1_b.size() >= 2 && triangles_2_b.size() < 2 ){
                    edgePosition = p1->point*(1.0f/2.0f) + p2->point*(1.0f/2.0f) ;
                }
                else {
                    p4 = GetTriangleAccident(p1,other_points[0],triangles_0[0]);
                    p5 = GetTriangleAccident(p2,other_points[0],triangles_0[0]);
                    p6 = GetTriangleAccident(p1,other_points[1],triangles_0[1]);
                    p7 = GetTriangleAccident(p2,other_points[1],triangles_0[1]);

                    glm::vec3 dir = (p2->point - p1->point);
                    edgePosition =  p1->point*(1.0f/2.0f)+p2->point*(1.0f/2.0f)+other_points[0]->point*(1.0f/8.0f)+other_points[1]->point*(1.0f/8.0f);
                    edgePosition +=p4->point*(-1.0f/16.0f)+p5->point*(-1.0f/16.0f)+p6->point*(-1.0f/16.0f)+p7->point*(-1.0f/16.0f);
                }

                edgePoint = GetPointerPoint(NPoints,edgePosition);
                v_edgePoint.push_back(edgePoint);
                v_vertexPoint.push_back(triangle->points[j]);
            }

            for(int j = 0;j < (int)v_vertexPoint.size();j++){
                Triangle_ptr _nTr = std::make_shared<Triangle_mesh>();
                int prev = (j) < 0 ? v_vertexPoint.size()-1 : j;
                int nxt = (j+2)%v_edgePoint.size();
                
                _nTr->points[0] = v_vertexPoint[j];
                _nTr->points[2] = v_edgePoint[nxt];
                _nTr->points[1] = v_edgePoint[prev];

                _nTr->points[0]->triangles.push_back(_nTr);
                _nTr->points[1]->triangles.push_back(_nTr);
                _nTr->points[2]->triangles.push_back(_nTr);
                subdivised_triangles.push_back(_nTr);
            }

            Triangle_ptr nTr = std::make_shared<Triangle_mesh>();
            for(int j=0;j<3;j++){
                nTr->points[j] = v_edgePoint[j];
                nTr->points[j]->triangles.push_back(nTr);
            }
            subdivised_triangles.push_back(nTr);
        }
        triangles.clear();
        triangles = std::vector<Triangle_ptr>(subdivised_triangles);
    }

    float* vertices = new float[subdivised_triangles.size()*6*3];
    unsigned int* indices_triangles = new unsigned int[subdivised_triangles.size()*3];
    int v_idx =0;
    int i_idx =0;
    for(int i = 0;i < subdivised_triangles.size();i++){
        glm::vec3 p1 = subdivised_triangles[i]->points[0]->point;
        glm::vec3 p2 = subdivised_triangles[i]->points[1]->point;
        glm::vec3 p3 = subdivised_triangles[i]->points[2]->point;
        glm::vec3 tangent = p1-p2;
        glm::vec3 bisnormal = p3-p1;
        glm::vec3 normal = glm::cross(bisnormal,tangent);

        for(int j = 0; j < 3;j++){
            vertices[v_idx++]= (subdivised_triangles[i]->points[j]->point.x);
            vertices[v_idx++]=(subdivised_triangles[i]->points[j]->point.y);
            vertices[v_idx++]=(subdivised_triangles[i]->points[j]->point.z);
            vertices[v_idx++]=(normal.x);
            vertices[v_idx++]=(normal.y);
            vertices[v_idx++]=(normal.z);
            indices_triangles[i_idx] = i_idx;
            i_idx++;
        }
        
    }

    CreateMesh(subdivised_triangles,mesh.GetName()+"_ButterFly");

    triangles.clear();
    subdivised_triangles.clear();
    points.clear();
    NPoints.clear();
}

void Subdivision::CreateMesh(std::vector<Triangle_ptr> subdivised_triangles,std::string name){
    float* vertices = new float[subdivised_triangles.size()*6*3];
    unsigned int* indices_triangles = new unsigned int[subdivised_triangles.size()*3];
    int v_idx =0;
    int i_idx =0;
    for(int i = 0;i < subdivised_triangles.size();i++){
        glm::vec3 p1 = subdivised_triangles[i]->points[0]->point;
        glm::vec3 p2 = subdivised_triangles[i]->points[1]->point;
        glm::vec3 p3 = subdivised_triangles[i]->points[2]->point;
        glm::vec3 tangent = p1-p2;
        glm::vec3 bisnormal = p3-p1;
        glm::vec3 normal = glm::cross(bisnormal,tangent);

        for(int j = 0; j < 3;j++){
            vertices[v_idx++]= (subdivised_triangles[i]->points[j]->point.x);
            vertices[v_idx++]=(subdivised_triangles[i]->points[j]->point.y);
            vertices[v_idx++]=(subdivised_triangles[i]->points[j]->point.z);
            vertices[v_idx++]=(normal.x);
            vertices[v_idx++]=(normal.y);
            vertices[v_idx++]=(normal.z);
            indices_triangles[i_idx] = i_idx;
            i_idx++;
        }
        
    }

    
    Mesh* mesh_subdivision = new Mesh(&vertices[0],sizeof(float)*v_idx,&indices_triangles[0],sizeof(unsigned int)*i_idx,6,name);
    this->obj->GetProjet()->AddPrimitive(mesh_subdivision);
    this->obj->SetMesh(mesh_subdivision);
}


void Subdivision::Kobalt(){
    Mesh mesh = *this->obj->GetMesh();
    unsigned int* indices = mesh.GetIndices(); 
    int size =  mesh.GetLenI()/sizeof(unsigned int);
    float epsilon = std::numeric_limits<float>::epsilon();
    std::vector<Triangle_ptr> triangles;
    std::vector<Point_ptr> points;

    // Intialize current Mesh
    for(int i = 0;i <size;i+=3){
        Triangle_ptr triangle = std::make_shared<Triangle_mesh>();
        for(int j=0;j<3;j++){
            Point_ptr p_triangle = GetPointerPoint(points,mesh.GetVerticesValue(indices[i+j]));
            triangle->points[j] = p_triangle;
            p_triangle->triangles.push_back(triangle);
        }
        triangle->barrycenter = (triangle->points[0]->point+triangle->points[1]->point+triangle->points[2]->point)/3.0f;
        triangles.push_back(triangle);
    }


    std::vector<Triangle_ptr> subdivised_triangles;
    for(int it = 0;it < this->iteration;it++){
        subdivised_triangles.clear();
        points.clear();
        for(int i = 0;i < triangles.size();i++){
            std::vector<Point_ptr> nVertex;
            Triangle_ptr current_triangles =(triangles[i]);
            for(int j=0;j < current_triangles->points.size();j++){
                Point_ptr p = current_triangles->points[j];
                std::vector<Point_ptr> accPts = GetAccident(p);
                float n = accPts.size();
                float alpha = (1.0f/9.0f)*(4.0f-(2.0f*(std::cosf(2.0f*glm::pi<float>()/n))));
                glm::vec3 perturbed_vertex = (1.0f-alpha)*p->point;
                for(int k = 0;k<n;k++ ){
                    perturbed_vertex += (alpha/n)*accPts[k]->point;
                }
                Point_ptr point = GetPointerPoint(points,perturbed_vertex);
                nVertex.push_back(point);
            }
        

            for(int j = 0;j < nVertex.size();j++){
                int nxt = (j+1)%nVertex.size();
                Point_ptr p1 = nVertex[j];
                Point_ptr p2 = nVertex[nxt];
                Point_ptr o_p1 = current_triangles->points[j];
                Point_ptr o_p2 = current_triangles->points[nxt];
                std::vector<Triangle_ptr> triangles_adj = GetTriangles(o_p1,o_p2);
                if(triangles_adj.size() > 1){
                    ProcessTrianglesCreation(p1,triangles_adj,current_triangles,triangles,points,subdivised_triangles,true);
                    ProcessTrianglesCreation(p2,triangles_adj,current_triangles,triangles,points,subdivised_triangles,false);
                }else{
                    Triangle_ptr t = std::make_shared<Triangle_mesh>();
                    t->points[0] = p1;
                    t->points[1] = p2;
                    t->points[2] = GetPointerPoint(points,current_triangles->barrycenter);

                    t->points[0]->triangles.push_back(t);
                    t->points[1]->triangles.push_back(t);
                    t->points[2]->triangles.push_back(t);

                    current_triangles->CreatedTriangles.push_back(t);
                    subdivised_triangles.push_back(t);
                    t->barrycenter = (t->points[0]->point+t->points[1]->point+t->points[2]->point)/3.0f;
                }
            }
        }
        triangles.clear();
        triangles = std::vector<Triangle_ptr>(subdivised_triangles);
    }

    float* vertices = new float[subdivised_triangles.size()*6*3];
    unsigned int* indices_triangles = new unsigned int[subdivised_triangles.size()*3];
    int v_idx =0;
    int i_idx =0;
    for(int i = 0;i < subdivised_triangles.size();i++){
        glm::vec3 p1 = subdivised_triangles[i]->points[0]->point;
        glm::vec3 p2 = subdivised_triangles[i]->points[1]->point;
        glm::vec3 p3 = subdivised_triangles[i]->points[2]->point;
        glm::vec3 tangent = p1-p2;
        glm::vec3 bisnormal = p3-p1;
        glm::vec3 normal = glm::cross(bisnormal,tangent);

        for(int j = 0; j < 3;j++){
            vertices[v_idx++]= (subdivised_triangles[i]->points[j]->point.x);
            vertices[v_idx++]=(subdivised_triangles[i]->points[j]->point.y);
            vertices[v_idx++]=(subdivised_triangles[i]->points[j]->point.z);
            vertices[v_idx++]=(normal.x);
            vertices[v_idx++]=(normal.y);
            vertices[v_idx++]=(normal.z);
            indices_triangles[i_idx] = i_idx;
            i_idx++;
        }
        
    }


    CreateMesh(subdivised_triangles,mesh.GetName()+"_kobolt");

    triangles.clear();
    subdivised_triangles.clear();
    points.clear();
    
}

void Subdivision::Catmull(){
    Mesh mesh = *this->obj->GetMesh();
    unsigned int* indices = mesh.GetIndices(); 
    int size =  mesh.GetLenI()/sizeof(unsigned int);
    float epsilon = std::numeric_limits<float>::epsilon();
    std::vector<Triangle_ptr> triangles;
    std::vector<Point_ptr> points;
    std::vector<Object*> lstObject;
    // Intialize current Mesh
    for(int i = 0;i <size;i+=3){
        Triangle_ptr triangle = std::make_shared<Triangle_mesh>();
        for(int j=0;j<3;j++){
            Point_ptr p_triangle = GetPointerPoint(points,mesh.GetVerticesValue(indices[i+j]));
            triangle->points[j] = p_triangle;
            p_triangle->triangles.push_back(triangle);
        }
        triangle->barrycenter = (triangle->points[0]->point+triangle->points[1]->point+triangle->points[2]->point)/3.0f;
        triangles.push_back(triangle);
    }
    std::vector<Triangle_ptr> subdivised_triangles;
    for(int it = 0; it < iteration;it++){
        subdivised_triangles.clear();
        points.clear();
        for(int i = 0;i<triangles.size();i++){
            std::vector<Point_ptr> v_edgePoint;
            std::vector<Point_ptr> v_vertexPoint;
            for(int j=0;j<triangles[i]->points.size();j++){
                int nxt = (j+1)%triangles[i]->points.size();
                Point_ptr p1 = triangles[i]->points[j];
                Point_ptr p2 = triangles[i]->points[nxt];
                std::vector<Triangle_ptr> tr = GetTriangles(p1,p2);

                glm::vec3 b1 = tr[0]->barrycenter;
                glm::vec3 b2 = tr[1]->barrycenter;
                glm::vec3 pos = (p1->point + p2->point + b1+b2)/4.0f;
                Point_ptr edgePoint = GetPointerPoint(points,pos);
                v_edgePoint.push_back(edgePoint);
                glm::vec3 vertex = glm::vec3(0);
                std::vector<Point_ptr> accidentPoints = GetAccident(p1);
                glm::vec3 Q = glm::vec3(0);
                glm::vec3 R = glm::vec3(0);
                float n = accidentPoints.size();
                for(int k=0;k<accidentPoints.size();k++){
                    glm::vec3 middir = (accidentPoints[k]->point-p1->point)/2.0f;
                    glm::vec3 mid = p1->point + middir;
                    R += ((1.0f)/n)*mid;
                }

                float n1 = p1->triangles.size();
                for(int k = 0;k<n1;k++){
                    Q += p1->triangles[k]->barrycenter;
                }
                Q/= n1;
                vertex = ((1.0f/n)*Q)+((2.0f/n)*R)+(((n-3.0f)/n)*p1->point);
                Point_ptr pVertex = GetPointerPoint(points,vertex);
                v_vertexPoint.push_back(pVertex);

            
            }
        
            int idx=0;
            for(int k = 0;k < v_edgePoint.size();k++ ){
                int prev = (k-1) < 0 ? v_edgePoint.size()-1:k-1;
                Point_ptr p1 = v_edgePoint[k];
                Point_ptr p2 = v_edgePoint[prev];
                Point_ptr v = v_vertexPoint[idx];
                Triangle_ptr tr = std::make_shared<Triangle_mesh>();
                tr->points[0] = p1;
                tr->points[1] = GetPointerPoint(points,triangles[i]->barrycenter);
                tr->points[2] = v;
                tr->points[0]->triangles.push_back(tr);
                tr->points[1]->triangles.push_back(tr);
                tr->points[2]->triangles.push_back(tr);
                tr->barrycenter = (tr->points[0]->point+tr->points[1]->point+tr->points[2]->point)/3.0f;
                subdivised_triangles.push_back(tr);

                Triangle_ptr tr_v = std::make_shared<Triangle_mesh>();
                tr_v->points[0] = p2;
                tr_v->points[2] = GetPointerPoint(points,triangles[i]->barrycenter);;
                tr_v->points[1] = v;
                tr_v->points[0]->triangles.push_back(tr_v);
                tr_v->points[1]->triangles.push_back(tr_v);
                tr_v->points[2]->triangles.push_back(tr_v);
                tr_v->barrycenter = (tr_v->points[0]->point+tr_v->points[1]->point+tr_v->points[2]->point)/3.0f;
                subdivised_triangles.push_back(tr_v);

                idx = (idx+1)%v_vertexPoint.size();
            }

        }
        triangles.clear();
        triangles = std::vector<Triangle_ptr>(subdivised_triangles);
    }
    CreateMesh(subdivised_triangles,mesh.GetName()+"_Catmull");

    triangles.clear();
    subdivised_triangles.clear();
    points.clear();
}

//

void Subdivision::Editor(){
     if (ImGui::BeginCombo("Subdivision Type", SubdivisionType[select_subdivision].c_str())) {
        for (int n = 0; n < sizeof(SubdivisionType)/sizeof(std::string); n++)
        {
            if (ImGui::Selectable(SubdivisionType[n].c_str())){
                //obj->SetMesh(projet->GetMesh(n));
                select_subdivision = n;
            }

        }
        ImGui::EndCombo();
    }

    ImGui::InputInt("Iteration",&this->iteration);
    this->iteration = std::max(1,this->iteration);
    
    if(select_subdivision == 0){
        ImGui::Checkbox("Smooth",&smooth);
    }

    if( ImGui::Button("Subdivise")){
        if(select_subdivision == 0){
            
            Loop();
        }

        if(select_subdivision == 1){
            Kobalt();
        }

        if(select_subdivision == 2){
            Catmull();
        }

         if(select_subdivision == 3){
            ButterFly();
        }


    }

   
 
};


void Subdivision::SetUp(){
    return;
}

void Subdivision::Update(){
   
    return;
}