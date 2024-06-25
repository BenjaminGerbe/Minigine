#include "../Headers/Subdivision.h"
#include "../Headers/Projet.h"
#include <limits>
RTTI_DEFINITION_BASE(Subdivision,Subdivision)


std::vector<Triangle_mesh*> Subdivision::GetTriangles(Point_mesh* a,Point_mesh* b){
        std::vector<Triangle_mesh*> result;
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

std::vector<Point_mesh*> Subdivision::ComputeTriangleLeftRightVertex(Point_mesh* a,Point_mesh* b){
    std::vector<Point_mesh*> result;
    std::vector<Triangle_mesh*> trianglesAccident = Subdivision::GetTriangles(a,b);
    int idx =0;
    for(int k = 0;k<trianglesAccident.size();k++){
        for(int i = 0;i < trianglesAccident[k]->points.size();i++){
            Point_mesh* point =trianglesAccident[k]->points[i];
            if(point != a && point != b){
                result.push_back(point);
            }
            
        }
    }
    
    return result;
}

std::vector<Point_mesh*> Subdivision::GetAccident(Point_mesh* a){
    std::vector<Point_mesh*> result;
    for(int i = 0; i < a->triangles.size();i++){
        for(int j = 0;j < a->triangles[i]->points.size();j++){
            Point_mesh* point = a->triangles[i]->points[j];
            if(point == a){
                continue;
            }
            result.push_back(point);
        }
    }

    return result;
}

Point_mesh* GetPointerPoint(std::vector<Point_mesh*>& points,glm::vec3 position){
    
    float epsilon = std::numeric_limits<float>::epsilon();
    Point_mesh* p_triangle = nullptr;
    for(int j = 0;j < points.size();j++){
        glm::vec3 point = points[j]->point;
        if( std::abs(glm::length((point - position))) < epsilon ){
            p_triangle = points[j];
        }

    }

    if(p_triangle == nullptr){
        p_triangle = new Point_mesh(position);
        points.push_back(p_triangle);
    }

    return p_triangle;
}

void Subdivision::Loop(){
    
    Mesh mesh = *this->obj->GetMesh();
    unsigned int* indices = mesh.GetIndices(); 
    int size =  mesh.GetLenI()/sizeof(unsigned int);
    std::cout << mesh.GetName() << " " << mesh.GetNbPoint()<< std::endl;
    //float* vertices = new float[mesh.GetNbPoint()*6.0f];
    float epsilon = std::numeric_limits<float>::epsilon();

    for(int i = 0;i <size;i+=3){
       
        Triangle_mesh* triangle = new Triangle_mesh();
        Point_mesh* p1_triangle = GetPointerPoint(points,mesh.GetVerticesValue(indices[i]));
        Point_mesh* p2_triangle = GetPointerPoint(points,mesh.GetVerticesValue(indices[i+1]));
        Point_mesh* p3_triangle = GetPointerPoint(points,mesh.GetVerticesValue(indices[i+2]));

        triangle->points[0] = p1_triangle;
        triangle->points[1] = p2_triangle;
        triangle->points[2] = p3_triangle;

        triangles.push_back(triangle);
        p1_triangle->triangles.push_back(triangle);
        p2_triangle->triangles.push_back(triangle);
        p3_triangle->triangles.push_back(triangle);
    }
    

    std::vector<Triangle_mesh*> subdivised_triangles;
    std::vector<Point_mesh*> NPoints;

    for(int it = 0;it < this->iteration;it++){
        subdivised_triangles.clear();
        NPoints.clear();
        for(int i = 0;i < triangles.size();i++){
            Triangle_mesh* triangle = triangles[i];

            // Calculate differentes edge Point and vertex points
            std::vector<Point_mesh*> v_edgePoint;
            std::vector<Point_mesh*> v_vertexPoint;
            for(int j = 0; j < triangle->points.size();j++){
            // glm::vec3 = ;
            int nxt = (j+1)%triangle->points.size();
            Point_mesh* p1 = triangle->points[j];
            Point_mesh* p2 = triangle->points[nxt];

            std::vector<Point_mesh*> other_points= ComputeTriangleLeftRightVertex(p1,p2);
            if(other_points.size() <= 1){
                glm::vec3 dir = (p2->point - p1->point);
                glm::vec3 edgePosition =  p1->point*(4.0f/9.0f)+p2->point*(4.0f/9.0f)+other_points[0]->point*(1.0f/9.0f);
                Point_mesh* edgePoint = GetPointerPoint(NPoints,edgePosition);
                v_edgePoint.push_back(edgePoint);
                std::vector<Point_mesh*> accidentPoints = GetAccident(p1);
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
                    }

                    vertexPosition = (1.0f-(n*alpha))*p1->point;
                    for(int k=0;k<n;k++){
                        vertexPosition += alpha*accidentPoints[k]->point;
                    }
                }
                
                
                Point_mesh* vertexPointer = GetPointerPoint(NPoints,vertexPosition);
                v_vertexPoint.push_back(vertexPointer);
            }
            else{
                glm::vec3 edgePosition = ((p1->point+p2->point)*(3.0f/8.0f))+
                (other_points[0]->point+other_points[1]->point)*(1.0f/8.0f);

                    Point_mesh* edgePoint = GetPointerPoint(NPoints,edgePosition);
                    v_edgePoint.push_back(edgePoint);

                    std::vector<Point_mesh*> accidentPoints = GetAccident(p1);
                    float alpha = 3.0/16.0f;
                    float n = accidentPoints.size();
                    if(n > 3){
                        float d = (3.0f/8.0f)+((1.0f/4.0f)*(std::cosf(2.0f*glm::pi<float>()/n)));
                        alpha = (1.0f/n)*( (5.0f/8.0f)-(d*d));
                    }

                    glm::vec3 vertexPosition = (1.0f-(n*alpha))*p1->point;
                    for(int k=0;k<n;k++){
                        vertexPosition += alpha*accidentPoints[k]->point;
                    }

                    Point_mesh* vertexPointer = GetPointerPoint(NPoints,vertexPosition);
                    v_vertexPoint.push_back(vertexPointer);

            }


            }


            for(int j = 0;j < v_vertexPoint.size();j++){
                Triangle_mesh* _nTr = new Triangle_mesh();
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

            Triangle_mesh* nTr = new Triangle_mesh();
            nTr->points[0] = v_edgePoint[0];
            nTr->points[1] = v_edgePoint[1];
            nTr->points[2] = v_edgePoint[2];

            nTr->points[0]->triangles.push_back(nTr);
            nTr->points[1]->triangles.push_back(nTr);
            nTr->points[2]->triangles.push_back(nTr);
            subdivised_triangles.push_back(nTr);


        }
        triangles.clear();
        triangles = std::vector<Triangle_mesh*>(subdivised_triangles);

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

    points.clear();

    Mesh* mesh_subdivision = new Mesh(&vertices[0],sizeof(float)*v_idx,&indices_triangles[0],sizeof(unsigned int)*i_idx,6,mesh.GetName()+"_subdived");
    triangles.clear();
    subdivised_triangles.clear();
    this->obj->GetProjet()->AddPrimitive(mesh_subdivision);


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

    if( ImGui::Button("Subdivise")){
        if(select_subdivision == 0){
            Loop();
        }
    }

 
};


void Subdivision::SetUp(){
    return;
}

void Subdivision::Update(){
   
    return;
}