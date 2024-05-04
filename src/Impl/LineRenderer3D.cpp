#include "../Headers/LineRenderer3D.h"
#include "../Headers/Projet.h"
#include "gtx/projection.hpp"
#include "gtx/vector_angle.hpp"
RTTI_DEFINITION_BASE(LineRenderer3D,LineRenderer3D)

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
int FindEdge(std::vector<Edge*> Edges,Edge* Edge){
    float epsilon = std::numeric_limits<float>::epsilon();
    int result = -1;
    for (int i = 0; i < Edges.size(); i++)
    {
        if(Edges[i] == Edge){
            continue;
        }

        if( glm::length(Edges[i]->v1->position - Edge->v1->position) < epsilon && glm::length(Edges[i]->v2->position - Edge->v2->position) < epsilon ||
            glm::length(Edges[i]->v2->position - Edge->v1->position) < epsilon && glm::length(Edges[i]->v2->position - Edge->v1->position) < epsilon ){
            result = i;
        }

    }

    return result;
}

int FindFace(std::vector<Face*> faces,Face* face){
    int result = -1;
    for (int i = 0; i < faces.size(); i++)
    {
        if(faces[i] == face){
            continue;
        }
        std::vector<Edge*> f({faces[i]->e1,faces[i]->e2,faces[i]->e3});
        int nb = 0;
        for (int k = 0; k < 3; k++)
        {
            if(FindEdge(f,(*faces[i])[k]) >= 0){
                nb++;
            }
        }
        if(nb >= 3){
            result = i;
            break;
        }

    }
    return result;
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
 if(stepTriangulation < 1 || lstObject.size() < 4  || lastIdxTriangulation >= lstObject.size()) return;
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

        Tetrahede* tetrahede = new Tetrahede();
        tetrahede->f1 = faces[0];
        tetrahede->f2 = faces[1];
        tetrahede->f3 = faces[2];
        tetrahede->f4 = faces[3];
        faces[0]->lstTetrahede.push_back(tetrahede);
        faces[1]->lstTetrahede.push_back(tetrahede);
        faces[2]->lstTetrahede.push_back(tetrahede);
        faces[3]->lstTetrahede.push_back(tetrahede);

        tetrahedes.push_back(tetrahede);
        lastIdxTriangulation += 4;
    }

    if(stepTriangulation > 1 && stepTriangulation != laststepTriangulation ){
        update = true;
        glm::vec3 P = lstObject[lastIdxTriangulation]->GetPosition();
        std::vector<Edge*> seeList;
        int faceSee = 0;
        std::vector<Edge*> unseeList;
        std::vector<Edge*> borderList;
        std::vector<Face*> facesSee;
        
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

            if(glm::dot(dir,normal) > 0 && faces[i]->visible != border){
                faces[i]->visible = see;
                facesSee.push_back(faces[i]);
                faceSee++;
            }
            else if (faces[i]->visible != border){
                faces[i]->visible = unsee;
            }
           
        }

        if(stepTriangulation % 2 != 0 && faceSee > 0 && stepTriangulation != laststepTriangulation){

            Vertex* nv = new Vertex();
            nv->position = P;
            vertices.push_back(nv);
            lastIdxTriangulation++;
            Edge* old = nullptr;
            std::vector<Edge*> cEdges;

            for (int i = 0; i < facesSee.size(); i++)
            {
                Face* face = facesSee[i];
                for (int k = 0; k < facesSee.size(); k++)
                {
                    if( k == i) continue;
                    for (int j = 0; j < 3; j++)
                    {
                        std::vector<Edge*> f({face->e1,face->e2,face->e3});
                        int idx = FindEdge(f,(*facesSee[k])[j]);
                        if(idx >= 0){
                            (*facesSee[k])[j]->visible = border;
                        }
                    }
                }
            }

            for (int i = 0; i < facesSee.size(); i++)
            {
                Tetrahede* tetrahede = new Tetrahede();
                facesSee[i]->visible = border;
                for (int j = 0; j < 3; j++)
                {
                    Edge* seg = (*facesSee[i])[j];
                    Edge* ne1 = new Edge();
                    ne1->v1 = seg->v1;
                    ne1->v2 = nv;

                    Edge* ne2 = new Edge();
                    ne2->v1 = seg->v2;
                    ne2->v2 = nv;

                    Edge* newEdge = new Edge();
                    newEdge->v1 = seg->v1;
                    newEdge->v2 = seg->v2;
                    edges.push_back(ne1);
                    edges.push_back(ne2);
                    edges.push_back(newEdge);

                    Face* face = new Face();
                    face->e1 = ne1;
                    face->e2 = ne2;
                    face->e3 = newEdge;
                    ne1->faces.push_back(face);
                    ne2->faces.push_back(face);
                    newEdge->faces.push_back(face);
                    tetrahede->SetFace(j,face);

                    int idx = FindFace(faces,face);
                    if(seg->visible == border){
                        face->visible = border;
                        faces[idx]->visible = border;
                    }
                    else{
                        faces.push_back(face);
                    }
                }

                Edge* seg = (*facesSee[i])[3];
                Edge* ne1 = new Edge();
                ne1->v1 = (*facesSee[i])[0]->v1;
                ne1->v2 = (*facesSee[i])[0]->v2;

                Edge* ne2 = new Edge();
                ne2->v1 = (*facesSee[i])[1]->v1;
                ne2->v2 = (*facesSee[i])[1]->v2;

                Edge* ne3 = new Edge();
                ne3->v1 = (*facesSee[i])[2]->v1;
                ne3->v2 = (*facesSee[i])[2]->v2;

               
                edges.push_back(ne1);
                edges.push_back(ne2);
                edges.push_back(ne3);

                Face* face = new Face();
                face->e1 = ne1;
                face->e2 = ne2;
                face->e3 = ne3;
                face->visible = border;

                ne1->faces.push_back(face);
                ne2->faces.push_back(face);
                ne3->faces.push_back(face);

                tetrahede->SetFace(3,face);
                faces.push_back(face);
                tetrahedes.push_back(tetrahede);
            }
            
            seeList.clear();
            facesSee.clear();
        }
        else if(faceSee == 0){
            lastIdxTriangulation++;
        }

    }

    if(stepTriangulation == laststepTriangulation) return;

    laststepTriangulation = stepTriangulation;

    if(lstPolygone.size() > 0){
        projet->GetScene()->RemoveObjectScene(lstPolygone);
        lstPolygone.clear();
    }

    
    if(stepTriangulation > 1 && lastIdxTriangulation < lstObject.size()){

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

    if(lastIdxTriangulation < lstObject.size()){
        Object* Vpoint = new Object(*projet->GetCube());
        Vpoint->SetName("Vertices");
        projet->GetScene()->AddObjectScene(Vpoint);
        Vpoint->SetPosition(lstObject[lastIdxTriangulation]->GetPosition());
        Vpoint->SetScale(glm::vec3({.2f,.2f,.2f}));

        Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-5]);
        
        lstPolygone.push_back(Vpoint);
    }

    for (int k = 0; k < tetrahedes.size(); k++)
    {
        for (int j = 0; j < 4; j++)
        {
            Face* f = (*tetrahedes[k])[j];
            for (int i = 0; i < 3; i++)
            {
                Edge* edge = (*f)[i];
                glm::vec3 a = edge->v1->position;
                glm::vec3 b = edge->v2->position;

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
                if(edge->visible == unsee ){
                    Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
                }
                else if (edge->visible == see){
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
        }
        
        
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

float GetCovariance(int x,int y,std::vector<glm::vec3> lstFloat ){
    float Xavg = 0;
    float Yavg = 0;

    for (int i = 0; i < lstFloat.size(); i++)
    {
        glm::vec3 point = lstFloat[i];
        Xavg += lstFloat[i][x];
        Yavg += lstFloat[i][y];
    }
    Xavg/= lstFloat.size();
    Yavg/= lstFloat.size();

    float result = 0;
    for (int i = 0; i < lstFloat.size(); i++)
    {
        result += (1.0f/lstFloat.size()) *((lstFloat[i][x] - Xavg)*lstFloat[i][y] - Yavg);       
    }

    return result;
}

float GetCovariance(int x,std::vector<glm::vec3> lstFloat ){
    float Xavg = 0;

    for (int i = 0; i < lstFloat.size(); i++)
    {
        glm::vec3 point = lstFloat[i];
        Xavg += lstFloat[i][x];
    }
    Xavg/= lstFloat.size();

    float result = 0;
    for (int i = 0; i < lstFloat.size(); i++)
    {
        result += (1.0f/lstFloat.size()) *std::pow((lstFloat[i][x] - Xavg),2.0f);       
    }

    return result;
}

void LineRenderer3D::CreateBone(){
    if(lstObject.size() <= 0) return;

    glm::vec3 barycentre(0,0,0);
    std::vector<glm::vec3> positions;
    for (int i = 0; i < lstObject.size(); i++)
    {
        barycentre += lstObject[i]->GetPosition();
        
    }
    barycentre/= lstObject.size();

    for (int i = 0; i < lstObject.size(); i++)
    {
        positions.push_back(lstObject[i]->GetPosition() - barycentre);
    }

    
    float x = GetCovariance(0,positions);
    float y = GetCovariance(1,positions);
    float z = GetCovariance(2,positions);
    float xy = GetCovariance(0,1,positions);
    float xz = GetCovariance(0,2,positions);
    float yz = GetCovariance(1,2,positions);

    glm::mat3 covarianceMatrice({
        x,xy,xz,
        xy,y,yz,
        xz,yz,z
    });

    // calculer le vecteur propre 
    glm::vec3 v(0,0,1);
    {
        glm::vec3 v1(0,0,1);
        int k = 0;
        float lk = 0.0f;
        float lk1 = 1.0f;
        float epsilon = std::numeric_limits<float>::epsilon();

        while(std::abs(lk - lk1) < epsilon || glm::length(v-v1) < epsilon){
            lk = lk1;
            v1 = v;
            glm::vec3 vt = covarianceMatrice*v;
            lk1 = std::max( std::max(std::abs(v.x),std::abs(v.y)),std::abs(v.z));
            v = (1.0f/lk)*vt;
        }
        v = glm::normalize(v);
    }

    std::cout << v.x << v.y << v.z << std::endl;

    float min = 0.0f;
    float max = 0.0f;
    for (int i = 0; i < positions.size(); i++)
    {
        float alpha = glm::dot(positions[i],v);
        glm::vec3 proj = v*alpha;
        if( alpha < min){
            min = alpha;
        }

        if( alpha > max){
            max = alpha;
        }
    }

    glm::vec3 a = barycentre+v*(min);
    glm::vec3 b = barycentre+v*(max);

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
    Line->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
    lstPolygone.push_back(Line);

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
    ImGui::InputInt("Step Triangulation",&this->stepTriangulation);
    ImGui::Spacing();
    if(ImGui::Button("Create Bone")){
        CreateBone();
    }

};


void LineRenderer3D::SetUp(){
    maxRadius = 5.0;
    nombrePoint = 5.0;
    update = false;
    laststep = -1;
    laststepTriangulation = -1;
    projet = this->obj->GetProjet();
    lastIdx = 0;
    lastIdxTriangulation= 0;

    return;
}

void LineRenderer3D::Update(){
   
     CreateConvexShape();
     TriangulationDelaunay();

    return;
}