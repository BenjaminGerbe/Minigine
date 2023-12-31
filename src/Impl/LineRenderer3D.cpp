#include "../Headers/LineRenderer3D.h"
#include "../Headers/Projet.h"
#include "gtx/projection.hpp"
#include "gtx/vector_angle.hpp"

void LineRenderer3D::GeneratePoint(){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (size_t i = 0; i < nombrePoint; i++)
    {
        Object* Point = new Object(*projet->GetCube());
        Point->SetName("Point");
        projet->GetScene()->AddObjectScene(Point);
        float r1 =  ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*2.f)-1.0f)*maxRadius;
        float r2 =  ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*2.f)-1.0)*maxRadius;
        float r3 =  ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*2.f)-1.0)*maxRadius;
        glm::vec3 pos( r1,r2,r3);
        Point->SetPosition(glm::vec3({pos.x,pos.y,pos.z}));
        Point->SetScale(glm::vec3({.2f,.2f,.2f}));
        lstObject.push_back(Point);
    }
    
}

void LineRenderer3D::CreateConvexShape(){
    if(step < 0 || lstObject.size() < 4) return;
    if(step == 1 && step != laststep){
        update = true;

        for (int i = 0; i < 4; i++)
        {
            Vertex* v = new Vertex();
            v->position = lstObject[i]->GetPosition();
            vertices.push_back(v);
        }
        for (int i = 0; i < vertices.size(); i++)
        {
            int nxt = (i+1)%vertices.size();
            Edge* e = new Edge();
            e->v1 = vertices[i];
            e->v2 = vertices[nxt];
            edges.push_back(e);
            vertices[i]->edges.push_back(e);
            vertices[nxt]->edges.push_back(e);
        }
        Edge* e = new Edge();
        e->v1 = vertices[0];
        e->v2 = vertices[2];
        edges.push_back(e);
        vertices[0]->edges.push_back(e);
        vertices[2]->edges.push_back(e);

        e = new Edge();
        e->v1 = vertices[1];
        e->v2 = vertices[3];
        edges.push_back(e);
        vertices[1]->edges.push_back(e);
        vertices[3]->edges.push_back(e);

        int faceIndices[4][3] = {
            {0, 1, 3},
            {0, 2, 4},
            {1, 2, 5},
            {3, 2, 5}
        };

        // Boucle pour créer les faces
        for (int i = 0; i < 4; i++)
        {
            Face* f = new Face();
            f->e1 = edges[faceIndices[i][0]];
            f->e2 = edges[faceIndices[i][1]];
            f->e3 = edges[faceIndices[i][2]];
            faces.push_back(f);
            edges[faceIndices[i][0]].push_back(f);
            edges[faceIndices[i][1]].push_back(f);
            edges[faceIndices[i][2]].push_back(f);
        }
        lastIdx += 4;
    }

    if(step != laststep){
        glm::vec3 P = lstObject[lastIdx]->GetPosition();
        for (int i = 0; i < faces.size(); i++)
        {
            glm::vec3 tangent = glm::normalize(faces[i]->e1->v2->position - faces[i]->e1->v1->position);
            glm::vec3 biNormal = glm::normalize(faces[i]->e2->v2->position - faces[i]->e2->v1->position);
            glm::vec3 normal = glm::cross(biNormal,tangent);

            int idx = 0;

            while(idx < vertices.size()){
                if(vertices[idx] != faces[i]->e1->v1 || vertices[idx] !=  faces[i]->e1->v2  ||
                vertices[idx] != faces[i]->e2->v1 || vertices[idx] !=  faces[i]->e2->v2 
                vertices[idx] != faces[i]->e3->v1 || vertices[idx] !=  faces[i]->e3->v2 
             ){
                break;
             }else{
                idx++;
             }
            }

            glm::vec3 dir = glm::normalize( vertices[idx]->position - faces[i]->e1->v1->position);
            if(glm::dot(dir,normal) < 0){
                normal = -nomal;
            }

            dir = glm::normalize( vertices[idx]->position - P);

            if(glm::dot(dir,normal) > 0){
                faces[i]->visible = see;
            }
            else{
                faces[i]->visible = unsee;
            }
        }

        for (int i = 0; i < edges.size(); i++)
        {
            Edge* e = edges[i];
            Visible v = edges[i]->visible;
            for (int k = 1; k < edges[i]->faces.size(); k++)
            {
                if( edges[i]->faces[k]->visible == see && v == unsee || edges[i]->faces[k]->visible == unsee && v == see ){
                    v = border;
                }
            }

            edges[i]->v1->visible = v;
            edges[i]->v2->visible = v;
        }

    }


    laststep = step;

    if(!update) return;

    if(lstPolygone.size() > 0){
        projet->GetScene()->RemoveObjectScene(lstPolygone);
        lstPolygone.clear();
    }

    // draw polygone
    for (int i = 0; i < vertices.size(); i++)
    {
        Object* Vpoint = new Object(*projet->GetCube());
        Vpoint->SetName("Point");
        projet->GetScene()->AddObjectScene(Vpoint);
        Vpoint->SetPosition(vertices[i]->position);
        Vpoint->SetScale(glm::vec3({.5f,.5f,.5f}));
        Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
        lstPolygone.push_back(Vpoint);
    }
    
    for (int i = 0; i < edges.size(); i++)
    {
        glm::vec3 a = edges[i]->v1->position;
        glm::vec3 b = edges[i]->v2->position;

        Object* Line = new Object(*projet->GetCube());
        Line->SetName("Line");

        glm::vec3 direction = glm::normalize(b - a);

        float ax = glm::degrees(std::asin(direction.y));
        float ay = glm::degrees(std::atan2(-direction.x, -direction.z));
        float az = 0.0f; 

        Line->SetPosition((a + b) * 0.5f);
        Line->SetRotation(glm::vec3(ax, ay, az));
        projet->GetScene()->AddObjectScene(Line);
        Line->SetScale(glm::vec3(0.2,0.2f,glm::length(b - a)));
        Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size() - 1]);
        lstPolygone.push_back(Line);
    }
    
    update = false;
}

void LineRenderer3D::Editor(){
    ImGui::InputFloat("MaxRadius",&this->maxRadius);
    ImGui::InputInt("Nombre Points",&this->nombrePoint);
    ImGui::Spacing();
    if(ImGui::Button("Generate Points")){
        GeneratePoint();
    }
    ImGui::Spacing();
    ImGui::InputInt("Step ConvexShape",&this->step);

};


void LineRenderer3D::SetUp(){
    maxRadius = 5.0;
    nombrePoint = 5.0;
    update = false;
    laststep = -1;
    projet = this->obj->GetProjet();
    lastIdx = 0;
    return;
}

void LineRenderer3D::Update(){
   
     CreateConvexShape();

    return;
}