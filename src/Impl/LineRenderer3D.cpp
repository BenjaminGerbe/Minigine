#include "../Headers/LineRenderer3D.h"
#include "../Headers/Projet.h"
#include "gtx/projection.hpp"
#include "gtx/vector_angle.hpp"

void LineRenderer3D::GeneratePoint(){
    //std::srand(static_cast<unsigned int>(std::time(nullptr)));
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
        Point->SetScale(glm::vec3({.15f,.15f,.15f}));
        Point->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-4]);
        lstObject.push_back(Point);
    }
    
}

bool same(Edge*a,Edge* b){

    float epsilon = std::numeric_limits<float>::epsilon();
    if( (glm::length(a->v1->position-b->v1->position) < epsilon &&
        glm::length(a->v2->position-b->v2->position) < epsilon )|| 
        (glm::length(a->v1->position-b->v2->position) < epsilon &&
        glm::length(a->v2->position-b->v1->position) < epsilon)
        ){
        return true;
    }

    return false;
}

void LineRenderer3D::TriangulationDelaunay(){
 if(stepTriangulation < 1 || lstObject.size() < 4  || lastIdx >= lstObject.size()) return;
    if(stepTriangulation == 1 && stepTriangulation != laststepTriangulation){
        update = true;

        for (int i = 0; i < 4; i++)
        {
            Vertex* v = new Vertex();
            v->position = lstObject[i]->GetPosition();
            vertices.push_back(v);
        }

        int verticesIndices[6][2] = {
            {0, 1},
            {0, 2},
            {0, 3},
            {1, 2},
            {1, 3},
            {2, 3},
        };

        for (int i = 0; i < 6; i++)
        {
            int nxt = (i+1)%vertices.size();
            Edge* e = new Edge();
            e->v1 = vertices[verticesIndices[i][0]];
            e->v2 = vertices[verticesIndices[i][1]];
            edges.push_back(e);
            vertices[verticesIndices[i][0]]->edges.push_back(e);
            vertices[verticesIndices[i][1]]->edges.push_back(e);
        }
     

        int faceIndices[4][3] = {
            {0, 1, 3},
            {0, 2, 4},
            {1, 2, 5},
            {3, 4, 5}
        };

        // Boucle pour créer les faces
        for (int i = 0; i < 4; i++)
        {
            Face* f = new Face();
            f->e1 = edges[faceIndices[i][0]];
            f->e2 = edges[faceIndices[i][1]];
            f->e3 = edges[faceIndices[i][2]];
            faces.push_back(f);
            edges[faceIndices[i][0]]->faces.push_back(f);
            edges[faceIndices[i][1]]->faces.push_back(f);
            edges[faceIndices[i][2]]->faces.push_back(f);
        }


        lastIdx += 4;
    }

    if(stepTriangulation > 1 && stepTriangulation != laststepTriangulation ){
        update = true;
        glm::vec3 P = lstObject[lastIdx]->GetPosition();
        std::vector<Edge*> seeList;
        int faceSee = 0;
        std::vector<Edge*> unseeList;
        std::vector<Edge*> borderList;

        
        for (int i = 0; i < faces.size(); i++)
        {
            std::vector<Vertex*> point;
            faces[i]->centerPosition= glm::vec3(0.0f);
            for (int j = 0; j < 3; j++)
            {   
                for (int k = 0; k < 2; k++)
                {
                    Face f = *faces[i];
                    Vertex* v = (*f[j])[k];
                    if(std::find(point.begin(),point.end(),v) == point.end()){
                        point.push_back(v);
                        faces[i]->centerPosition += v->position;
                    }
                }
            }

            faces[i]->centerPosition /=3.0f; 

            glm::vec3 tangent = glm::normalize(point[1]->position - point[0]->position);
            glm::vec3 biNormal = glm::normalize(point[2]->position -  point[0]->position);
            glm::vec3 normal = glm::cross(biNormal,tangent);

          

            int idx = 0;   

            while(idx < vertices.size()){
                if(std::find(point.begin(),point.end(),vertices[idx]) == point.end()){
                    break;
                }
                else{
                    idx++;
                }
            }

            glm::vec3 dir = glm::normalize(  vertices[idx]->position - faces[i]->e1->v1->position );
            if(glm::dot(dir,normal) > 0){
                normal = -normal;
            }

            dir = glm::normalize( P - faces[i]->e1->v1->position);

            if(glm::dot(dir,normal) > 0){
                faces[i]->visible = see;
                faceSee++;
            }
            else{
                faces[i]->visible = unsee;
            }
           
        }

        for (int i = 0; i < edges.size(); i++)
        {
            Edge* e = edges[i];
            e->visible = edges[i]->faces[0]->visible;
            if( edges[i]->faces[0]->visible != edges[i]->faces[1]->visible){
                e->visible = border;
                borderList.push_back(e);
            }

            if(e->visible == see){
                seeList.push_back(e);
            }

            if(e->visible == unsee){
                unseeList.push_back(e);
            }

        }

        for (int i = 0; i < unseeList.size(); i++)
        {
            if(unseeList[i]->visible == unsee){
                unseeList[i]->v1->visible = unsee;
                unseeList[i]->v2->visible = unsee;
            }
        }
        
        for (int i = 0; i < seeList.size(); i++)
        {
            if(seeList[i]->visible == see){
                seeList[i]->v1->visible = see;
                seeList[i]->v2->visible = see;
            }
        }

        for (int i = 0; i < borderList.size(); i++)
        {
            if(borderList[i]->visible == border){
                borderList[i]->v1->visible = border;
                borderList[i]->v2->visible = border;
            }
        }

        if(stepTriangulation % 2 != 0 && faceSee > 0){

            Vertex* nv = new Vertex();
            nv->position = P;
            vertices.push_back(nv);
            lastIdx++;
            Edge* old = nullptr;
            std::vector<Edge*> cEdges;
            std::vector<Face*> facesDelete;
            for (int i = 0; i < borderList.size(); i++)
            {
                for (int j =  borderList[i]->faces.size() - 1; j >= 0.0; j--)
                {
                    Face* f = borderList[i]->faces[j];
                    if(f->visible == see){
                        for (int k = 0; k < faces.size(); k++)
                        {
                            if(faces[k] == f){
                                faces.erase(faces.begin()+k);
                            }
                        }
                        if(std::find(facesDelete.begin(),facesDelete.end(),f) == facesDelete.end()){
                            facesDelete.push_back(f);
                        }
                        borderList[i]->faces.erase( borderList[i]->faces.begin()+j);
                    }
                }
                
            }

            for (int i = 0; i < seeList.size(); i++)
            {
                for (int j =  seeList[i]->faces.size() - 1; j >= 0.0; j--)
                {
                    Face* f = seeList[i]->faces[j];
                    if(f->visible == see){
                        for (int k = 0; k < faces.size(); k++)
                        {
                            if(faces[k] == f){
                                faces.erase(faces.begin()+k);
                            }
                        }
                        if(std::find(facesDelete.begin(),facesDelete.end(),f) == facesDelete.end()){
                            facesDelete.push_back(f);
                        }
                        seeList[i]->faces.erase( seeList[i]->faces.begin()+j);
                    }
                }
                
            }

            for (int i = 0; i < facesDelete.size(); i++)
            {
                delete facesDelete[i];
            }
            
            facesDelete.clear();


            for (int i = 0; i < borderList.size(); i++)
            {
                Edge* ne1 = new Edge();
                ne1->v1 = borderList[i]->v1;
                ne1->v2 = nv;

                Edge* ne2 = new Edge();
                ne2->v1 = borderList[i]->v2;
                ne2->v2 = nv;

                bool ae1 = true;
                bool ae2 = true;

                for (int i = 0; i < cEdges.size(); i++)
                {
                    if(same(cEdges[i],ne1)){
                        delete ne1;
                        ne1 = cEdges[i];
                        ae1 = false;
                    }

                    if(same(cEdges[i],ne2)){
                        delete ne2;
                        ne2 = cEdges[i];
                        ae2 = false;
                    }
                }
                
                Face* face = new Face();
                face->e1 = ne1;
                face->e2 = ne2;
                face->e3 = borderList[i];
                face->centerPosition =( borderList[i]->v1->position+ borderList[i]->v2->position + nv->position) /3.0f;
                ne1->faces.push_back(face);
                ne2->faces.push_back(face);
                borderList[i]->faces.push_back(face);

                if(ae1){
                    nv->edges.push_back(ne1);
                    borderList[i]->v1->edges.push_back(ne1);
                    cEdges.push_back(ne1);
                    edges.push_back(ne1);
                } 
                    

                if(ae2){
                    nv->edges.push_back(ne2);
                    borderList[i]->v2->edges.push_back(ne2);
                    cEdges.push_back(ne2);
                    edges.push_back(ne2);
                }

                faces.push_back(face);
            }
            
            for (int i = edges.size()-1; i >=0; i--)
            {
                if(edges[i]->visible == see){
                    edges[i]->faces.clear();
                    delete edges[i];
                    edges.erase(edges.begin()+i);
                }
            }

            for (int i = vertices.size()-1; i >=0; i--)
            {
                if(vertices[i]->visible == see){
                    delete vertices[i];
                    vertices.erase(vertices.begin()+i);
                }
            }
            
            seeList.clear();
        }
        else if(faceSee == 0){
            lastIdx++;
        }

    }

    if(stepTriangulation == laststepTriangulation) return;

    laststepTriangulation = stepTriangulation;

    if(lstPolygone.size() > 0){
        projet->GetScene()->RemoveObjectScene(lstPolygone);
        lstPolygone.clear();
    }

    // draw polygone
    for (int i = 0; i < vertices.size(); i++)
    {
        Object* Vpoint = new Object(*projet->GetCube());
        Vpoint->SetName("Vertices");
        projet->GetScene()->AddObjectScene(Vpoint);
        Vpoint->SetPosition(vertices[i]->position);
        Vpoint->SetScale(glm::vec3({.2f,.2f,.2f}));

        Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-4]);
        if(vertices[i]->visible == unsee){
            Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
        }
        else if (vertices[i]->visible == see){
            Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-2]);
        }
        else if (vertices[i]->visible == border){
            Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-3]);
        }

        if(lastIdx >= lstObject.size()){
            Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
        }
        lstPolygone.push_back(Vpoint);

    }

    if(lastIdx < lstObject.size()){
        Object* Vpoint = new Object(*projet->GetCube());
        Vpoint->SetName("Vertices");
        projet->GetScene()->AddObjectScene(Vpoint);
        Vpoint->SetPosition(lstObject[lastIdx]->GetPosition());
        Vpoint->SetScale(glm::vec3({.2f,.2f,.2f}));

        Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-5]);
        
        lstPolygone.push_back(Vpoint);
    }

    if(stepTriangulation > 1 && laststepTriangulation < lstObject.size()){

        for (int i = 0; i < faces.size(); i++)
        {
            Object* Vpoint = new Object(*projet->GetCube());
            Vpoint->SetName("Faces");
            projet->GetScene()->AddObjectScene(Vpoint);
            Vpoint->SetPosition(faces[i]->centerPosition);
            Vpoint->SetScale(glm::vec3({.1f,.1f,.1f}));

            Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-4]);
            if(faces[i]->visible == unsee  ){
                Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
            }
            else if (faces[i]->visible == see){
                Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-2]);
            }
            else if (faces[i]->visible == border){
                Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-3]);
            }
            lstPolygone.push_back(Vpoint);
        }
    }
    
    for (int i = 0; i < edges.size(); i++)
    {
        glm::vec3 a = edges[i]->v1->position;
        glm::vec3 b = edges[i]->v2->position;

        Object* Line = new Object(*projet->GetCube());
        Line->SetName("Edgess");

        glm::vec3 direction = glm::normalize(b - a);

        float ax = glm::degrees(std::asin(direction.y));
        float ay = glm::degrees(std::atan2(-direction.x, -direction.z));
        float az = 0.0f; 

        Line->SetPosition((a + b) * 0.5f);
        Line->SetRotation(glm::vec3(ax, ay, az));
        projet->GetScene()->AddObjectScene(Line);
        Line->SetScale(glm::vec3(0.1,0.1f,glm::length(b - a)));
        Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-4]);
        if(edges[i]->visible == unsee ){
            Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
        }
        else if (edges[i]->visible == see){
            Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-2]);
        }
        else if (edges[i]->visible == border){
            Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-3]);
        }

        if(lastIdx >= lstObject.size()){
            Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
        }
        lstPolygone.push_back(Line);
    }
    
    update = false;
}

void LineRenderer3D::CreateConvexShape(){
    if(step < 1 || lstObject.size() < 4  || lastIdx >= lstObject.size()) return;
    if(step == 1 && step != laststep){
        update = true;

        for (int i = 0; i < 4; i++)
        {
            Vertex* v = new Vertex();
            v->position = lstObject[i]->GetPosition();
            vertices.push_back(v);
        }

        int verticesIndices[6][2] = {
            {0, 1},
            {0, 2},
            {0, 3},
            {1, 2},
            {1, 3},
            {2, 3},
        };

        for (int i = 0; i < 6; i++)
        {
            int nxt = (i+1)%vertices.size();
            Edge* e = new Edge();
            e->v1 = vertices[verticesIndices[i][0]];
            e->v2 = vertices[verticesIndices[i][1]];
            edges.push_back(e);
            vertices[verticesIndices[i][0]]->edges.push_back(e);
            vertices[verticesIndices[i][1]]->edges.push_back(e);
        }
     

        int faceIndices[4][3] = {
            {0, 1, 3},
            {0, 2, 4},
            {1, 2, 5},
            {3, 4, 5}
        };

        // Boucle pour créer les faces
        for (int i = 0; i < 4; i++)
        {
            Face* f = new Face();
            f->e1 = edges[faceIndices[i][0]];
            f->e2 = edges[faceIndices[i][1]];
            f->e3 = edges[faceIndices[i][2]];
            faces.push_back(f);
            edges[faceIndices[i][0]]->faces.push_back(f);
            edges[faceIndices[i][1]]->faces.push_back(f);
            edges[faceIndices[i][2]]->faces.push_back(f);
        }
        lastIdx += 4;
    }

    if(step > 1 && step != laststep ){
        update = true;
        glm::vec3 P = lstObject[lastIdx]->GetPosition();
        std::vector<Edge*> seeList;
        int faceSee = 0;
        std::vector<Edge*> unseeList;
        std::vector<Edge*> borderList;

        
        for (int i = 0; i < faces.size(); i++)
        {
            std::vector<Vertex*> point;
            faces[i]->centerPosition= glm::vec3(0.0f);
            for (int j = 0; j < 3; j++)
            {   
                for (int k = 0; k < 2; k++)
                {
                    Face f = *faces[i];
                    Vertex* v = (*f[j])[k];
                    if(std::find(point.begin(),point.end(),v) == point.end()){
                        point.push_back(v);
                        faces[i]->centerPosition += v->position;
                    }
                }
            }

            faces[i]->centerPosition /=3.0f; 

            glm::vec3 tangent = glm::normalize(point[1]->position - point[0]->position);
            glm::vec3 biNormal = glm::normalize(point[2]->position -  point[0]->position);
            glm::vec3 normal = glm::cross(biNormal,tangent);

          

            int idx = 0;   

            while(idx < vertices.size()){
                if(std::find(point.begin(),point.end(),vertices[idx]) == point.end()){
                    break;
                }
                else{
                    idx++;
                }
            }

            glm::vec3 dir = glm::normalize(  vertices[idx]->position - faces[i]->e1->v1->position );
            if(glm::dot(dir,normal) > 0){
                normal = -normal;
            }

            dir = glm::normalize( P - faces[i]->e1->v1->position);

            if(glm::dot(dir,normal) > 0){
                faces[i]->visible = see;
                faceSee++;
            }
            else{
                faces[i]->visible = unsee;
            }
           
        }

        for (int i = 0; i < edges.size(); i++)
        {
            Edge* e = edges[i];
            e->visible = edges[i]->faces[0]->visible;
            if( edges[i]->faces[0]->visible != edges[i]->faces[1]->visible){
                e->visible = border;
                borderList.push_back(e);
            }

            if(e->visible == see){
                seeList.push_back(e);
            }

            if(e->visible == unsee){
                unseeList.push_back(e);
            }

        }

        for (int i = 0; i < unseeList.size(); i++)
        {
            if(unseeList[i]->visible == unsee){
                unseeList[i]->v1->visible = unsee;
                unseeList[i]->v2->visible = unsee;
            }
        }
        
        for (int i = 0; i < seeList.size(); i++)
        {
            if(seeList[i]->visible == see){
                seeList[i]->v1->visible = see;
                seeList[i]->v2->visible = see;
            }
        }

        for (int i = 0; i < borderList.size(); i++)
        {
            if(borderList[i]->visible == border){
                borderList[i]->v1->visible = border;
                borderList[i]->v2->visible = border;
            }
        }

        if(step % 2 != 0 && faceSee > 0){

            Vertex* nv = new Vertex();
            nv->position = P;
            vertices.push_back(nv);
            lastIdx++;
            Edge* old = nullptr;
            std::vector<Edge*> cEdges;
            std::vector<Face*> facesDelete;
            for (int i = 0; i < borderList.size(); i++)
            {
                for (int j =  borderList[i]->faces.size() - 1; j >= 0.0; j--)
                {
                    Face* f = borderList[i]->faces[j];
                    if(f->visible == see){
                        for (int k = 0; k < faces.size(); k++)
                        {
                            if(faces[k] == f){
                                faces.erase(faces.begin()+k);
                            }
                        }
                        if(std::find(facesDelete.begin(),facesDelete.end(),f) == facesDelete.end()){
                            facesDelete.push_back(f);
                        }
                        borderList[i]->faces.erase( borderList[i]->faces.begin()+j);
                    }
                }
                
            }

            for (int i = 0; i < seeList.size(); i++)
            {
                for (int j =  seeList[i]->faces.size() - 1; j >= 0.0; j--)
                {
                    Face* f = seeList[i]->faces[j];
                    if(f->visible == see){
                        for (int k = 0; k < faces.size(); k++)
                        {
                            if(faces[k] == f){
                                faces.erase(faces.begin()+k);
                            }
                        }
                        if(std::find(facesDelete.begin(),facesDelete.end(),f) == facesDelete.end()){
                            facesDelete.push_back(f);
                        }
                        seeList[i]->faces.erase( seeList[i]->faces.begin()+j);
                    }
                }
                
            }

            for (int i = 0; i < facesDelete.size(); i++)
            {
                delete facesDelete[i];
            }
            
            facesDelete.clear();


            for (int i = 0; i < borderList.size(); i++)
            {
                Edge* ne1 = new Edge();
                ne1->v1 = borderList[i]->v1;
                ne1->v2 = nv;

                Edge* ne2 = new Edge();
                ne2->v1 = borderList[i]->v2;
                ne2->v2 = nv;

                bool ae1 = true;
                bool ae2 = true;

                for (int i = 0; i < cEdges.size(); i++)
                {
                    if(same(cEdges[i],ne1)){
                        delete ne1;
                        ne1 = cEdges[i];
                        ae1 = false;
                    }

                    if(same(cEdges[i],ne2)){
                        delete ne2;
                        ne2 = cEdges[i];
                        ae2 = false;
                    }
                }
                
                Face* face = new Face();
                face->e1 = ne1;
                face->e2 = ne2;
                face->e3 = borderList[i];
                face->centerPosition =( borderList[i]->v1->position+ borderList[i]->v2->position + nv->position) /3.0f;
                ne1->faces.push_back(face);
                ne2->faces.push_back(face);
                borderList[i]->faces.push_back(face);

                if(ae1){
                    nv->edges.push_back(ne1);
                    borderList[i]->v1->edges.push_back(ne1);
                    cEdges.push_back(ne1);
                    edges.push_back(ne1);
                } 
                    

                if(ae2){
                    nv->edges.push_back(ne2);
                    borderList[i]->v2->edges.push_back(ne2);
                    cEdges.push_back(ne2);
                    edges.push_back(ne2);
                }

                faces.push_back(face);
            }
            
            for (int i = edges.size()-1; i >=0; i--)
            {
                if(edges[i]->visible == see){
                    edges[i]->faces.clear();
                    delete edges[i];
                    edges.erase(edges.begin()+i);
                }
            }

            for (int i = vertices.size()-1; i >=0; i--)
            {
                if(vertices[i]->visible == see){
                    delete vertices[i];
                    vertices.erase(vertices.begin()+i);
                }
            }
            
            seeList.clear();
        }
        else if(faceSee == 0){
            lastIdx++;
        }

    }

    if(step == laststep) return;

    laststep = step;

    if(lstPolygone.size() > 0){
        projet->GetScene()->RemoveObjectScene(lstPolygone);
        lstPolygone.clear();
    }

    // draw polygone
    for (int i = 0; i < vertices.size(); i++)
    {
        Object* Vpoint = new Object(*projet->GetCube());
        Vpoint->SetName("Vertices");
        projet->GetScene()->AddObjectScene(Vpoint);
        Vpoint->SetPosition(vertices[i]->position);
        Vpoint->SetScale(glm::vec3({.2f,.2f,.2f}));

        Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-4]);
        if(vertices[i]->visible == unsee){
            Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
        }
        else if (vertices[i]->visible == see){
            Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-2]);
        }
        else if (vertices[i]->visible == border){
            Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-3]);
        }

        if(lastIdx >= lstObject.size()){
            Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
        }
        lstPolygone.push_back(Vpoint);

    }

    if(lastIdx < lstObject.size()){
        Object* Vpoint = new Object(*projet->GetCube());
        Vpoint->SetName("Vertices");
        projet->GetScene()->AddObjectScene(Vpoint);
        Vpoint->SetPosition(lstObject[lastIdx]->GetPosition());
        Vpoint->SetScale(glm::vec3({.2f,.2f,.2f}));

        Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-5]);
        
        lstPolygone.push_back(Vpoint);
    }

    if(step > 1 && lastIdx < lstObject.size()){

        for (int i = 0; i < faces.size(); i++)
        {
            Object* Vpoint = new Object(*projet->GetCube());
            Vpoint->SetName("Faces");
            projet->GetScene()->AddObjectScene(Vpoint);
            Vpoint->SetPosition(faces[i]->centerPosition);
            Vpoint->SetScale(glm::vec3({.1f,.1f,.1f}));

            Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-4]);
            if(faces[i]->visible == unsee  ){
                Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
            }
            else if (faces[i]->visible == see){
                Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-2]);
            }
            else if (faces[i]->visible == border){
                Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-3]);
            }
            lstPolygone.push_back(Vpoint);
        }
    }
    
    for (int i = 0; i < edges.size(); i++)
    {
        glm::vec3 a = edges[i]->v1->position;
        glm::vec3 b = edges[i]->v2->position;

        Object* Line = new Object(*projet->GetCube());
        Line->SetName("Edgess");

        glm::vec3 direction = glm::normalize(b - a);

        float ax = glm::degrees(std::asin(direction.y));
        float ay = glm::degrees(std::atan2(-direction.x, -direction.z));
        float az = 0.0f; 

        Line->SetPosition((a + b) * 0.5f);
        Line->SetRotation(glm::vec3(ax, ay, az));
        projet->GetScene()->AddObjectScene(Line);
        Line->SetScale(glm::vec3(0.1,0.1f,glm::length(b - a)));
        Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-4]);
        if(edges[i]->visible == unsee ){
            Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
        }
        else if (edges[i]->visible == see){
            Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-2]);
        }
        else if (edges[i]->visible == border){
            Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-3]);
        }

        if(lastIdx >= lstObject.size()){
            Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
        }
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
    ImGui::InputInt("Step Triangulation",&this->step);

};


void LineRenderer3D::SetUp(){
    maxRadius = 5.0;
    nombrePoint = 5.0;
    update = false;
    laststep = -1;
    laststepTriangulation = -1;
    projet = this->obj->GetProjet();
    lastIdx = 0;

    return;
}

void LineRenderer3D::Update(){
   
     CreateConvexShape();

    return;
}