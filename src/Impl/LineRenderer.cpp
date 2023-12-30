#include "../Headers/LineRenderer.h"
#include "../Headers/Projet.h"

void FindSegment(std::vector<Triangle*> tr,Segment* s){

    float epsilon = std::numeric_limits<float>::epsilon();
    for (int i = 0; i < tr.size(); i++)
    {
        Triangle triangle = *tr[i];
        for (int k = 0; k < 3; k++)
        {
            if( (glm::length(triangle[k]->A-s->A) < epsilon &&
              glm::length(triangle[k]->B-s->B) < epsilon )|| 
              (glm::length(triangle[k]->A-s->B) < epsilon &&
              glm::length(triangle[k]->B-s->A) < epsilon)
              ){
               s->visible = false;
               triangle[k]->visible = false;
            }
        }
        
    }
}

void setVisibility(std::vector<Triangle*> tr,Segment* s,Triangle* nor){

    float epsilon = std::numeric_limits<float>::epsilon();
    for (int i = 0; i < tr.size(); i++)
    {
        Triangle triangle = *tr[i];
        if(tr[i] == nor) continue;
        for (int k = 0; k < 3; k++)
        {
            if( (glm::length(triangle[k]->A-s->A) < epsilon &&
              glm::length(triangle[k]->B-s->B) < epsilon )|| 
              (glm::length(triangle[k]->A-s->B) < epsilon &&
              glm::length(triangle[k]->B-s->A) < epsilon)
              ){
               s->visible = false;
               triangle[k]->visible = false;
            }
        }
        
    }
}


int FindSegment(std::vector<Segment*> Segments,glm::vec3 A,glm::vec3 B){
    int result = -1;
    float epsilon = std::numeric_limits<float>::epsilon();
    for (int i = 0; i < Segments.size(); i++)
    {
        Segment* seg = Segments[i];
        if( (glm::length(seg->A-A) < epsilon &&
            glm::length(seg->B-B) < epsilon )|| 
            (glm::length(seg->A-B) < epsilon &&
            glm::length(seg->B-A) < epsilon)
            ){
                result = i;
        }
    }

    return result;
}


int FindTriangle(std::vector<Triangle*> tr,Segment* s){

    int result = -1;
    float epsilon = std::numeric_limits<float>::epsilon();
    for (int i = 0; i < tr.size(); i++)
    {
        Triangle triangle = *tr[i];
        for (int k = 0; k < 3; k++)
        {
            if( (glm::length(triangle[k]->A-s->A) < epsilon &&
              glm::length(triangle[k]->B-s->B) < epsilon )|| 
              (glm::length(triangle[k]->A-s->B) < epsilon &&
              glm::length(triangle[k]->B-s->A) < epsilon)
              ){
               result = i;
            }
        }
        
    }
    return result;
}

int FindTriangle(std::vector<Triangle*> tr,Segment* s,Triangle* nor){

    int result = -1;
    float epsilon = std::numeric_limits<float>::epsilon();
    for (int i = 0; i < tr.size(); i++)
    {
        Triangle triangle = *tr[i];
        if(tr[i] == nor) continue;
        for (int k = 0; k < 3; k++)
        {
            if( (glm::length(triangle[k]->A-s->A) < epsilon &&
              glm::length(triangle[k]->B-s->B) < epsilon )|| 
              (glm::length(triangle[k]->A-s->B) < epsilon &&
              glm::length(triangle[k]->B-s->A) < epsilon)
              ){
               result = i;
            }
        }
        
    }
    return result;
}

bool isSame(Segment* a,Segment* b){
    float epsilon = std::numeric_limits<float>::epsilon();
    if( (glm::length(a->A-b->A) < epsilon &&
        glm::length(a->B-b->B) < epsilon )|| 
        (glm::length(a->A-b->B) < epsilon &&
        glm::length(a->B-b->A) < epsilon)
        ){
        return true;
    }

    return false;
}

void DeleteSegment(std::vector<Segment*> segs,Segment* s){
    int result = -1;
    for (int i = 0; i < segs.size(); i++)
    {
        if(s == segs[i]){
            result = i;
            break;
        }
    }

    segs.erase(segs.begin()+result);
}

bool isContainTriangle(Triangle* tr,glm::vec3 point){
    
    bool contain = true;
    for (int i = 0; i < 3; i++)
    {
        Segment* seg = tr->GetSegment(i);
        glm::vec3 A = seg->A;
        glm::vec3 B = seg->B;

        glm::vec3 d = B-A;            
        glm::vec3 normal = glm::normalize(glm::vec3(d.y,-d.x,0.0));
        glm::vec3 dir = glm::normalize(A - point);

        if(glm::dot(dir,normal) < 0){
            contain = false;
            break;
        }
    }
    return contain;
}

std::vector<Segment*> visibleSegment(std::vector<Segment*>Segments,glm::vec3 point){
    std::vector<Segment*> L;
    //std::cout << "P"<<" = (" << point.x << ","<<point.y << " )" <<std::endl; 
    for (int i = 0; i < Segments.size(); i++)
    {
        Segment* seg = Segments[i];
        if(seg->visible == false) continue;
        glm::vec3 A = seg->A;
        glm::vec3 B = seg->B;

      //  std::cout << "A_{"<<i << "} = (" << seg->A.x << ","<<seg->A.y << " )" <<std::endl; 
       // std::cout << "B_{"<<i << "}  = (" << seg->B.x << ","<<seg->B.y << " )" <<std::endl; 

        glm::vec3 d = B-A;            
        glm::vec3 normal = glm::normalize(glm::vec3(d.y,-d.x,0.0));
        glm::vec3 dir = glm::normalize(A - point);

        if(glm::dot(dir,normal) < 0){
            L.push_back(seg);
        }

    }

   
    return L;
}

void LineRenderer::SuppressionDelaunay(glm::vec3 mousePosition){
    
    if(lstObject.size() < 0) return;
    float epsilon = std::numeric_limits<float>::epsilon();

    int deletedIdx = -1;
    for (int i = 0; i < lstObject.size(); i++)
    {
        if(glm::length(lstObject[i]->GetPosition() - mousePosition) <= 2.0){
            deletedIdx = i;
        }
    }
    if(deletedIdx< 0){
        return;
    }

    glm::vec3 s = lstObject[deletedIdx]->GetPosition();
    projet->GetScene()->RemoveObjectScene(lstObject[deletedIdx]);
    lstObject.erase(lstObject.begin()+deletedIdx);

    if(trianglesPoints.size() < 0) return;
    std::vector<int> trianglesToDelete;
    std::vector<Segment*> segmentToKeep;
    std::vector<Segment*> garbage;

    for (int i = 0; i < trianglesPoints.size(); i++)
    {
        Triangle* tr = trianglesPoints[i];
        std::vector<Segment*> segmentToKeepTmp;
        bool accident = false;
        for (int j = 0; j < 3; j++)
        {
            Segment* seg = tr->GetSegment(j);

            if(glm::length(seg->A - s) < epsilon || glm::length(seg->B - s) < epsilon ){
                if(std::find(trianglesToDelete.begin(), trianglesToDelete.end(), i) == trianglesToDelete.end()) {
                    trianglesToDelete.push_back(i);
                }
                accident = true;
            }else{
                segmentToKeepTmp.push_back(seg);
            }
            
        }

        if(accident == true){
           segmentToKeep.insert(segmentToKeep.end(),segmentToKeepTmp.begin(),segmentToKeepTmp.end());
        }
        
    }
    
   for (int i = trianglesToDelete.size()-1; i >= 0 ; i--)
   {
        Triangle* triangle = trianglesPoints[trianglesToDelete[i]];
        trianglesPoints.erase(trianglesPoints.begin()+trianglesToDelete[i]);
        delete triangle;
   }
   int idx = -1;
   //CreateLine(segmentToKeep);
    
    if(segmentToKeep.size() <= 2){
       int i = FindSegment(Segments,segmentToKeep[0]->A,segmentToKeep[0]->B);
       Segments[i]->visible = true;
       i = FindSegment(Segments,segmentToKeep[1]->A,segmentToKeep[1]->B);
       Segments[i]->visible = true;
    }

   while(segmentToKeep.size() >= 3){
        idx = (idx+1)%segmentToKeep.size();
        Segment* segment = segmentToKeep[idx];
        
        
        // premier sommet du segment
        for (int i = 0; i < segmentToKeep.size(); i++)
        {
            if(segment == segmentToKeep[i]) continue;
            bool isBalanced = true;
            Segment* seg = new Segment();
            seg->A = segmentToKeep[i]->A;
            seg->B = segmentToKeep[i]->B;
            seg->visible = segmentToKeep[i]->visible;

            if(glm::length(segment->A-seg->A) < epsilon ){
                std::swap(seg->A,seg->B);
            }

            if(glm::length(segment->A-seg->B) < epsilon ){
                //std::cout << "U_{"<<idx << "} = (" << seg->A.x << ","<<seg->A.y << " )" <<std::endl; 
                //std::cout << "V_{"<<idx << "}  = (" << seg->B.x << ","<<seg->B.y << " )" <<std::endl; 
                glm::vec3 A = seg->A;
                glm::vec3 B = segment->A;
                glm::vec3 C = segment->B;


                for (int k = 0; k < segmentToKeep.size(); k++)
                {

                    if(!(glm::length(A - segmentToKeep[k]->A) < epsilon || glm::length(B - segmentToKeep[k]->A) < epsilon || glm::length(C - segmentToKeep[k]->A) < epsilon)){
                        glm::vec3 D = segmentToKeep[k]->A;
                        glm::mat3 mat({
                        A.x,A.y,1.0,
                        B.x,B.y,1.0,
                        C.x,C.y,1.0,
                        });

                        if(glm::determinant(mat) < 0 ){
                            std::swap(A,C);
                        }

                         
                        glm::mat4 m = glm::mat4({
                            A.x,A.y,std::pow(A.x,2)+std::pow(A.y,2),1,
                            B.x,B.y,std::pow(B.x,2)+std::pow(B.y,2),1,
                            C.x,C.y,std::pow(C.x,2)+std::pow(C.y,2),1,
                            D.x,D.y,std::pow(D.x,2)+std::pow(D.y,2),1
                        });

                        if(glm::determinant(m)>0){
                            isBalanced = false;
                            break;
                        }
                    }

                }

                if(!isBalanced){
                    delete seg;
                    break;
                }
                
                Triangle* tr = new Triangle();
                tr->SetSegment(0,segment);
                Segment* a = new Segment();
                a->A = segment->B;
                a->B = seg->A;

                tr->SetSegment(1,a);
                tr->SetSegment(2,seg);
                trianglesPoints.push_back(tr);

                Segment* a2 = segmentToKeep[i];
                segmentToKeep.erase(segmentToKeep.begin()+idx);
                int del = FindSegment(segmentToKeep,a2->A,a2->B);
                segmentToKeep.erase(segmentToKeep.begin()+del);
                del = FindSegment(segmentToKeep,a->A,a->B);
                if(del < 0){
                    Segment* tmp = new Segment();
                    tmp->A = a->B;
                    tmp->B = a->A;
                    tmp->visible = a->visible;
                    garbage.push_back(tmp);
                    segmentToKeep.push_back(tmp);
                }
                else{
                    segmentToKeep.erase(segmentToKeep.begin()+del);
                }
            }
        }
        for (int i = 0; i < segmentToKeep.size(); i++)
        {
            std::cout << "A_{"<<i << "} = (" << segmentToKeep[i]->A.x << ","<<segmentToKeep[i]->A.y << " )" <<std::endl; 
            std::cout << "B_{"<<i << "} = (" << segmentToKeep[i]->B.x << ","<<segmentToKeep[i]->B.y << " )" <<std::endl; 
        }
        
   }

    Segments.clear();
    for (int i = 0; i < trianglesPoints.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Segment* seg = trianglesPoints[i]->GetSegment(j);
            Segments.push_back(seg);

            setVisibility(trianglesPoints,seg,trianglesPoints[i]);
        }
        
    }

    for(auto p:garbage){
        delete p;
    }

}

void LineRenderer::TriangulationDelaunay(){
    if(lstObject.size() <= 1) return;

    float epsilon = std::numeric_limits<float>::epsilon();
    int size = lstObject.size();
    
    if(lstLines.size() > 0){
        projet->GetScene()->RemoveObjectScene(lstLines);
        lstLines.clear();
    }        

    if(lstObject.size() <= 2){
        Segment* seg = new Segment();
        seg->A = lstObject[0]->GetPosition();
        seg->B = lstObject[1]->GetPosition();
        Segments.push_back(seg);
        CreateLine(Segments);
        return;
    }

    int nPoint = size-1;
    if(trianglesPoints.size() <= 0){
        Triangle* tr = new Triangle();
        tr->SetSegment(0,Segments[0]);
        glm::vec3 newPoint =  lstObject[nPoint]->GetPosition();;
        glm::mat3 mat({
        Segments[0]->A.x,Segments[0]->A.y,1.0,
        newPoint.x,newPoint.y,1.0,
        Segments[0]->B.x,Segments[0]->B.y,1.0,
        });

        if(glm::determinant(mat) > 0 ){
            std::swap(Segments[0]->A,Segments[0]->B);

        }

        Segment* seg = new Segment();
        seg->A = Segments[0]->B;
        seg->B = newPoint;
        Segments.push_back(seg);
        tr->SetSegment(1,seg);
        seg = new Segment();
        seg->A = newPoint;
        seg->B = Segments[0]->A;
        Segments.push_back(seg);
        tr->SetSegment(2,seg);
        trianglesPoints.push_back(tr);
        //CreateLine(Segments);
        return;
    }

    std::vector<Segment*> L = visibleSegment(Segments,lstObject[nPoint]->GetPosition());
    
    if(L.size() == 0){
        int triangleToDelete = -1;
        for (int i = 0; i < trianglesPoints.size(); i++)
        {
            if(isContainTriangle(trianglesPoints[i],lstObject[nPoint]->GetPosition())){
                for (int k = 0; k < 3; k++)
                {
                    L.push_back(trianglesPoints[i]->GetSegment(k));
                }
                triangleToDelete = i;
                break;
            }
        }
    

        Triangle* triangle = trianglesPoints[triangleToDelete];
        trianglesPoints.erase(trianglesPoints.begin()+triangleToDelete);
        delete triangle;
    }

    while(L.size() > 0){
        Segment* a = L[L.size()-1];
        L.pop_back();


        int idxTR = FindTriangle(trianglesPoints,a);
        glm::mat4 m;
        Triangle* tr;
        glm::vec3 newPoint = lstObject[nPoint]->GetPosition();
        
        if(idxTR >= 0){

            tr = trianglesPoints[idxTR];
            glm::vec3 A = a->A;
            glm::vec3 B;
            glm::vec3 C = a->B;
            glm::vec3 D = lstObject[nPoint]->GetPosition();
           
         
            for (int k = 0; k < 3; k++)
            {
                if(glm::length(A - (*tr)[k]->A) > epsilon && glm::length(C - (*tr)[k]->A) > epsilon ){
                    B = (*tr)[k]->A;
                    break;
                }

                if(glm::length(A - (*tr)[k]->B) > epsilon && glm::length(C - (*tr)[k]->B) > epsilon ){
                    B = (*tr)[k]->B;
                    break;
                }
            }


            glm::mat3 mat({
            A.x,A.y,1.0,
            B.x,B.y,1.0,
            C.x,C.y,1.0,
            });

            if(glm::determinant(mat) < 0 ){
                std::swap(A,C);
            }
        
            m = glm::mat4({
                A.x,A.y,std::pow(A.x,2)+std::pow(A.y,2),1,
                B.x,B.y,std::pow(B.x,2)+std::pow(B.y,2),1,
                C.x,C.y,std::pow(C.x,2)+std::pow(C.y,2),1,
                D.x,D.y,std::pow(D.x,2)+std::pow(D.y,2),1
            });

            std::cout << glm::determinant(m) << std::endl;
        }

        if( idxTR >= 0 && glm::determinant(m)>0 ){
            
            trianglesPoints.erase(trianglesPoints.begin()+idxTR);
            int idxdel = 0;
            glm::vec3 s1 = a->A;
            glm::vec3 s2 = a->B;
            for (int i = 0; i < 3; i++)
            {
                if(!isSame(tr->GetSegment(i),a)){
                    L.push_back(tr->GetSegment(i));
                }
                tr->GetSegment(i)->visible = true;
            }

            do
            {
                idxdel= FindSegment(Segments,s1,s2);
                if(idxdel < 0) break;
                Segment* s = Segments[idxdel];
                Segments.erase(Segments.begin()+idxdel);
                delete s;
            } while (idxdel >= 0);
            
            delete tr;
        
        }
        else{
            Triangle* nTr = new Triangle();

            Segment* f = new Segment();
            f->A = a->A;
            f->B = a->B;
         

            glm::mat3 mat({
            f->A.x,f->A.y,1.0,
            newPoint.x,newPoint.y,1.0,
            f->B.x,f->B.y,1.0,
            });

            if(glm::determinant(mat) > 0 ){
                std::swap(f->A,f->B);
            }

            nTr->SetSegment(0,f);
            if(idxTR >= 0){
                f->visible  = false;
                a->visible  = false;
            }
            else{
               int idxdel = 0;
                do
                {
                    idxdel= FindSegment(Segments,a->A,a->B);
                    if(idxdel < 0) break;
                    Segment* s = Segments[idxdel];
                    Segments.erase(Segments.begin()+idxdel);
                    delete s;
                } while (idxdel >= 0);
            }

            Segments.push_back(f);

            Segment* seg = new Segment();
            seg->A = f->B;
            seg->B = newPoint;
            nTr->SetSegment(1,seg);
            int k = FindSegment(Segments,f->B,newPoint);
            if(k >= 0){
                Segments[k]->visible = false;
                seg->visible = false;
            }
            Segments.push_back(seg);

            seg = new Segment();
            seg->A = newPoint;
            seg->B = f->A;
            nTr->SetSegment(2,seg);

            k = FindSegment(Segments,f->A,newPoint);
            if(k >= 0){
                Segments[k]->visible = false;
                seg->visible = false;
            }
            Segments.push_back(seg);

            trianglesPoints.push_back(nTr);
        }
    }

    //CreateLine(Segments);
    
    
    
    return;
}

 void LineRenderer::Triangulation(){
    float xmin = lstObject[0]->GetPosition().x;
    float ymin = lstObject[0]->GetPosition().y;
    float epsilon = std::numeric_limits<float>::epsilon();
    int size = lstObject.size();
    std::vector<Object*> points(lstObject);
    std::vector<Triangle*> lstTriangles;
    std::vector<Segment*> VisibleSegments;
    std::vector<Segment*> garbage;
    bool finish = false;

    while(!finish){
        finish = true;
        for (int i = 0; i < lstObject.size()-1; i++)
        {
            glm::vec3 pt0 = points[i]->GetPosition();
            glm::vec3 pt1 = points[i+1]->GetPosition();
            
            if(pt1.x < pt0.x || ( std::abs( pt1.x - pt0.x) < epsilon && pt1.y < pt0.y)){
                std::swap(points[i],points[i+1]);
                finish = false;
            }
            
        }
    }

    Triangle* Trgle = new Triangle();
    //First Triangle
   
    glm::vec3 pt1 = points[0]->GetPosition();
    glm::vec3 pt2 = points[1]->GetPosition();
    glm::vec3 pt3 = points[2]->GetPosition();

    glm::mat3 mat({
    pt1.x,pt1.y,1.0,
    pt2.x,pt2.y,1.0,
    pt3.x,pt3.y,1.0,
    });
//
    if(glm::determinant(mat) < 0 ){
        std::swap(pt1,pt2);
    }

    Segment* seg1F = new Segment();
    seg1F->A = pt1;
    seg1F->B = pt2;
    Segment* seg2F = new Segment();
    seg2F->A = pt2;
    seg2F->B = pt3;
    Segment* seg3F = new Segment();
    seg3F->A = pt3;
    seg3F->B = pt1;

    garbage.push_back(seg1F);
    garbage.push_back(seg2F);
    garbage.push_back(seg3F);

    Trgle->SetSegment(0,seg1F);
    Trgle->SetSegment(1,seg2F);
    Trgle->SetSegment(2,seg3F);

    lstTriangles.push_back(Trgle);

    for (int i = 0; i < 3; i++)
        points.erase(points.begin()+0);
    
    for (int i = 0; i < (int)points.size(); i++)
    {
        glm::vec3 position = points[i]->GetPosition();
       // std::cout<<" P = (" << position.x << ","<<position.y << " )"<< std::endl;
        for (int j = (int)lstTriangles.size()-1; j >= 0 ; j--)
        {
            Triangle triangle = *lstTriangles[j];
            for (int k = 0; k < 3; k++)
            {
                Segment* seg = triangle.GetSegment(k);
                
                if(!seg->visible){
                    continue;
                }

                glm::vec3 A = seg->A;
                glm::vec3 B = seg->B;
                glm::vec3 C = position;
                glm::vec3 d = B-A;
                glm::vec3 normal = glm::normalize(glm::vec3(d.y,-d.x,0.0));
                glm::vec3 dir = glm::normalize(A - position);

                glm::mat3 mat({
                A.x,A.y,1.0,
                C.x,C.y,1.0,
                B.x,B.y,1.0,
                });
            //
                if(glm::determinant(mat) < 0 && glm::dot(dir,normal) > 0){
                    continue;
                }

                Triangle* _triangle = new Triangle();
               
                Segment* seg1 = new Segment();
                seg1->A = A;
                seg1->B = C;
                Segment* seg2 = new Segment();
                seg2->A = C;
                seg2->B = B;

                garbage.push_back(seg1);
                garbage.push_back(seg2);

                if(Delaunay){
                    
                    glm::vec3 _A = A;
                    glm::vec3 _B = triangle.GetSegment((k+1)%3)->B;
                    glm::vec3 _C = B;
                    glm::vec3 _D = C;

                 
                    glm::mat4 m({
                        _A.x,_A.y,std::pow(_A.x,2)+std::pow(_A.y,2),1,
                        _D.x,_D.y,std::pow(_D.x,2)+std::pow(_D.y,2),1,
                        _C.x,_C.y,std::pow(_C.x,2)+std::pow(_C.y,2),1,
                        _B.x,_B.y,std::pow(_B.x,2)+std::pow(_B.y,2),1
                    });

                     

                    if(glm::determinant(m)>0 ){

                        Segment* s1 = triangle.GetSegment(0); 
                        Segment* s2 = triangle.GetSegment(1); 
                        Segment* s3 = triangle.GetSegment(2); 
                        s1->A = _A;
                        s1->B = _D;
                        s2->A = _D;
                        s2->B = _B;
                        s2->visible = false;
                        s3->A = _B;
                        s3->B = _A;

                        seg =  new Segment();
                        garbage.push_back(seg);
                        seg->A = _B;
                        seg->B = _D;
                        seg1->A = _D;
                        seg1->B = _C;
                        seg2->A = _C;
                        seg2->B = _B;
                      
                    }
                }

                FindSegment(lstTriangles,seg);
                FindSegment(lstTriangles,seg1);
                FindSegment(lstTriangles,seg2);

                _triangle->SetSegment(0,seg);  
                _triangle->SetSegment(1,seg1);
                _triangle->SetSegment(2,seg2);

                seg->visible = false;
                lstTriangles.push_back(_triangle);
            }
            
        }
    }
    

    CreateLineTriangle(lstTriangles);

    for(auto p:garbage){
        delete p;
    }

    for(auto p:lstTriangles){
        delete p;
    }

    return;
 }

void LineRenderer::GrahamScan(){

    // CreatePolygone

    if(lstObject.size() <= 0)
        return;

    glm::vec3 v({1.0f,0.0f,0.0f});

    std::vector<PolyPoint> Poly;
    float size = lstObject.size();
    float epsilon = std::numeric_limits<float>::epsilon();
    glm::vec3 B({0.0f,0.0f,0.0f});

    for (int i = 0; i < (int)size; i++)
    {
        B += lstObject[i]->GetPosition();
    }

    B = glm::vec3(B.x/size,B.y/size,B.y/size);

    
    for (int i = 0; i < lstObject.size(); i++)
    {
        glm::vec3 a = lstObject[i]->GetPosition();
        glm::vec b = B + v;
        glm::vec3 pp = a-B;

        float l = glm::length(pp);
        pp = glm::normalize(pp);
        float dot = glm::dot(v,pp);
        float angle = std::acosf(dot);

        glm::mat3 mat({
            a.x,a.y,1.0,
            B.x,B.y,1.0,
            b.x,b.y,1.0,
        });

        if(glm::determinant(mat) < 0){
            angle = (glm::pi<float>()*2.0f)-angle;
        }

        PolyPoint p;
        p.angle = angle;
        p.length = l;
        p.idx= i;
        p.position = a;

        Poly.push_back(p);
        if(Poly.size() <= 1)
            continue;

        int c = Poly.size()-1;
        for (int i = 0; i < Poly.size(); i++)
        {
            if( Poly[c].angle < Poly[i].angle ||
            ( (Poly[c].angle - Poly[i].angle) < epsilon && Poly[c].length < Poly[i].length) ){
                PolyPoint tmp = Poly[c];
                Poly[c] = Poly[i];
                Poly[i] = tmp;
            }
        }

    }
    
    int nonConvex = false;
    while(!nonConvex){
        nonConvex = true;
        int i = Poly.size()-1;
        while(i >=0){
            int prec = i-1;
            int nxt =( i+1)%Poly.size();
            if(prec < 0){
                prec = Poly.size() -1;
            }

            glm::vec a = Poly[prec].position;
            glm::vec3 b = Poly[i].position;
            glm::vec3 c = Poly[nxt].position;
            
            glm::mat3 mat({
                a.x,a.y,1.0,
                b.x,b.y,1.0,
                c.x,c.y,1.0,
            });

            if(glm::determinant(mat) < 0 ){
                nonConvex = false;
                Poly.erase(Poly.begin() +i);
            }

            i--;
        }
    }
    

    std::vector<Object*> Points;
    for (int i = 0; i < Poly.size(); i++)
    {
        Points.push_back(lstObject[Poly[i].idx]);
    }
    

    Object* A = new Object(*projet->GetEmpty());
    A->SetProjet(projet);
    LineRenderer* lr = new LineRenderer(Points,A);
    A->SetObjectType(o_LineRenderer);
    A->AddComponent(lr);
    lr->SetUp();
    A->SetName("Graham-scan");
  
    projet->GetScene()->AddObjectScene(A);
    return;
}

void LineRenderer::GiftWraping(){

    if(lstObject.size() <= 0)
        return;

    // GiftWraping logic :

    // find the first pivot :
    float xmin = lstObject[0]->GetPosition().x;
    float ymin = lstObject[0]->GetPosition().y;
    int first = 0;
    float epsilon = std::numeric_limits<float>::epsilon();
    int size = lstObject.size();
    for (int i = 1; i < lstObject.size(); i++)
    {
        Object* o = lstObject[i];
        glm::vec3 position = o->GetPosition();

        if(position.x < xmin || ( std::abs(position.x - xmin) < epsilon  && position.y < ymin)){
            xmin = position.x;
            ymin = position.y;
            first = i;
        }
            
    }

    // initialization 
    glm::vec3 v({0.0,-1.0f,0.0});
    std::vector<Object*> Pivots;
    int i = first;

    // Wrapping logic
    do{
        Pivots.push_back(new Object(*lstObject[i]));

        glm::vec3 pp;
        float lmax = std::numeric_limits<float>::min();
        float dot = glm::dot(v,pp);
        float angleMin = std::numeric_limits<float>::max();
        int I = 0;

        for (int j = 0; j < lstObject.size(); j++)
        {
            int idx = j;
            if(idx != i){
                glm::vec3 b = lstObject[idx]->GetPosition();
                glm::vec3 a = lstObject[i]->GetPosition();
                glm::vec3 pp = b-a;

                float l = glm::length(pp);
                pp = glm::normalize(pp);
                dot = glm::dot(v,pp);
                float angle = std::acosf(dot);

                if(angle < angleMin || ( std::abs(angle - angleMin) ) < epsilon && l > lmax ){
                    lmax = l;
                    angleMin = angle;
                    I = idx;
                }

            }
        }

        v =  lstObject[I]->GetPosition() - lstObject[i]->GetPosition();
        i = I;
        v = glm::normalize(v);
    }while(i != first && Pivots.size() <= size);

    if(Pivots.size() > size){
        std::cout << "error loop" << std::endl;
        assert(false);
    }
    
    Object* A = new Object(*projet->GetEmpty());
    A->SetProjet(projet);
    LineRenderer* lr = new LineRenderer(Pivots,A);
    A->SetObjectType(o_LineRenderer);
    A->AddComponent(lr);
    lr->SetUp();
    A->SetName("Gift-Wraping");
    projet->GetScene()->AddObjectScene(A);

    for(Object* o : Pivots){
        delete o;
    }
}

void LineRenderer::Editor(){
    ImGui::Checkbox("RenderLine",&RenderLine);
    ImGui::SameLine();
    ImGui::Checkbox("Delaunay",&Delaunay);
    ImGui::Spacing();
    ImGui::Checkbox("DelaunayTriangulation",&DelaunayTriangulation);
    ImGui::Spacing();
    ImGui::Checkbox("SupressionDelaunay",&supression);
    ImGui::Spacing();
    ImGui::Checkbox("RenderVoronoi",&Voronoi);
    ImGui::Spacing();
    if(ImGui::Button("Gift Wrapping")){
        GiftWraping();
    }
    ImGui::Spacing();
    if(ImGui::Button("Graham-Scan")){
        GrahamScan();
    }
    ImGui::Spacing();
    if(ImGui::Button("Triangulation")){
        Triangulation();
    }
    ImGui::Spacing();
    if(ImGui::Button("ClearTriangulation")){
        projet->GetScene()->RemoveObjectScene(SegmentTriangles);
        SegmentTriangles.clear();
    }

       
    return;
}
void LineRenderer::SetUp(){
    projet = this->obj->GetProjet();
    lastIdx = 0;
    start = true;
    Delaunay = false;
    DelaunayTriangulation = true;
    supression = false;

    time = 0.0;
    
    return;
}

void LineRenderer::RenderVoronoi(){
    if(lstLinesVornoi.size() > 0){
        projet->GetScene()->RemoveObjectScene(lstLinesVornoi);
        lstLinesVornoi.clear();
    }

    if(trianglesPoints.size() < 1) return;

    for (int i = 0; i < trianglesPoints.size(); i++)
    {

        glm::vec3 A = trianglesPoints[i]->GetSegment(0)->A;
        glm::vec3 B = trianglesPoints[i]->GetSegment(1)->A;
        glm::vec3 C = trianglesPoints[i]->GetSegment(2)->A;

        float lA = glm::length(A);
        float lB = glm::length(B);
        float lC = glm::length(C);

        glm::mat3 mat1({
            lA*lA,A.y,1,
            lB*lB,B.y,1,
            lC*lC,C.y,1,
        });

        glm::mat3 mat2({
            A.x,lA*lA,1,
            B.x,lB*lB,1,
            C.x,lC*lC,1,
        });

        float a = glm::determinant( glm::mat3({
            A.x,A.y,1,
            B.x,B.y,1,
            C.x,C.y,1,
        }));


        glm::vec3 S(0.5f*glm::determinant(mat1)/a,0.5f*glm::determinant(mat2)/a,0.0f);
        
        trianglesPoints[i]->circumcircle = S;
    

    }

    for (int i = 0; i < trianglesPoints.size(); i++)
    {
        Object* Vpoint = new Object(*projet->GetCube());
        Vpoint->SetName("Point");
        projet->GetScene()->AddObjectScene(Vpoint);
        Vpoint->SetPosition(trianglesPoints[i]->circumcircle);
        Vpoint->SetScale(glm::vec3({.8f,.8f,.8f}));
        Vpoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
        lstLinesVornoi.push_back(Vpoint);

        for (int j = 0; j < 3; j++)
        {
            int k = FindTriangle(trianglesPoints,trianglesPoints[i]->GetSegment(j),trianglesPoints[i]);
            glm::vec3 a = trianglesPoints[i]->circumcircle;
            glm::vec3 b;

            if(k < 0){
                glm::vec3 A = trianglesPoints[i]->GetSegment(j)->A;
                glm::vec3 B = trianglesPoints[i]->GetSegment(j)->B;
                glm::vec3 d = B-A;            
                glm::vec3 normal = glm::normalize(glm::vec3(d.y,-d.x,0.0));
                b = a + normal * 100.0f;

            }
            else{
                b = trianglesPoints[k]->circumcircle;
            }
           

            Object* VornoiPoint = new Object(*projet->GetCube());
            VornoiPoint->SetName("Vornoi");
            glm::vec3 vec = a-b;
            float normal = glm::length(vec);
            vec = glm::normalize(vec);
            VornoiPoint->SetScale(glm::vec3(0.2,normal,0.2));
            glm::vec3 c = glm::vec3(0,1,0);
            float dot = glm::dot(vec,c);
            float angle = std::acosf(dot);
            c = a + c;
            glm::mat3 mat({
                a.x,a.y,1.0,
                b.x,b.y,1.0,
                c.x,c.y,1.0,
            });
            if(glm::determinant(mat) < 0){
                angle =-angle;
            }
            VornoiPoint->SetPosition(a - vec*(normal/2.0f));
            VornoiPoint->SetRotation(glm::vec3({0.0f,0.0f, angle * (180.0/glm::pi<float>()) }));
            projet->GetScene()->AddObjectScene(VornoiPoint);
            VornoiPoint->SetMaterial(projet->GetMaterials()[projet->GetMaterials().size()-1]);
            lstLinesVornoi.push_back(VornoiPoint);
        }

    }


}


void LineRenderer::CreateLineTriangle(std::vector<Triangle*>lstTriangles){
    
    if(SegmentTriangles.size() > 0 ){

        projet->GetScene()->RemoveObjectScene(SegmentTriangles);
        SegmentTriangles.clear();
    }

    if(lstTriangles.size() <= 0 ) 
        return;

    for (int i = 0; i < (int)lstTriangles.size(); i++)
    {
        for (int k = 0; k < 3; k++)
        {
            glm::vec3 a = lstTriangles[i]->GetSegment(k)->A;
            glm::vec3 b = lstTriangles[i]->GetSegment(k)->B;

            Object* A = new Object(*projet->GetCube());
            A->SetName("Line");
            glm::vec3 vec = a-b;
            float normal = glm::length(vec);
            vec = glm::normalize(vec);
            A->SetScale(glm::vec3(0.2,normal,0.2));
            glm::vec3 c = glm::vec3(0,1,0);
            float dot = glm::dot(vec,c);
            float angle = std::acosf(dot);
            c = a + c;
            glm::mat3 mat({
                a.x,a.y,1.0,
                b.x,b.y,1.0,
                c.x,c.y,1.0,
            });
            if(glm::determinant(mat) < 0){
                angle =-angle;
            }
            A->SetPosition(a - vec*(normal/2.0f));
            A->SetRotation(glm::vec3({0.0f,0.0f, angle * (180.0/glm::pi<float>()) }));
            projet->GetScene()->AddObjectScene(A);
            SegmentTriangles.push_back(A);
        }
        
    }
}

void LineRenderer::CreateLine(std::vector<Segment*> segment){

    if(lstObject.size() <= 1 ) 
        return;

    if(lstLines.size() > 0){
        projet->GetScene()->RemoveObjectScene(lstLines);
        lstLines.clear();
    }

    for (int i = 0; i < segment.size(); i++)
    {
        glm::vec3 a = segment[i]->A;
        glm::vec3 b = segment[i]->B;

        Object* A = new Object(*projet->GetCube());
        A->SetName("Line");
        glm::vec3 vec = a-b;
        float normal = glm::length(vec);
        vec = glm::normalize(vec);
        A->SetScale(glm::vec3(0.2,normal,0.2));
        glm::vec3 c = glm::vec3(0,1,0);
        float dot = glm::dot(vec,c);
        float angle = std::acosf(dot);
        c = a + c;
        glm::mat3 mat({
            a.x,a.y,1.0,
            b.x,b.y,1.0,
            c.x,c.y,1.0,
        });
        if(glm::determinant(mat) < 0){
            angle =-angle;
        }
        A->SetPosition(a - vec*(normal/2.0f));
        A->SetRotation(glm::vec3({0.0f,0.0f, angle * (180.0/glm::pi<float>()) }));
        projet->GetScene()->AddObjectScene(A);
        lstLines.push_back(A);
    }

    return;
}

void LineRenderer::CreateLine(){


    if(lstObject.size() <= 1 ) 
        return;

    if(lstLines.size() > 0){
        projet->GetScene()->RemoveObjectScene(lstLines);
        lstLines.clear();
    }

    for (int i = 0; i < lstObject.size(); i++)
    {
        glm::vec3 a = lstObject[i]->GetPosition();
        glm::vec3 b = lstObject[(i+1)%lstObject.size()]->GetPosition();

        Object* A = new Object(*projet->GetCube());
        A->SetName("Line");
        glm::vec3 vec = a-b;
        float normal = glm::length(vec);
        vec = glm::normalize(vec);
        A->SetScale(glm::vec3(0.2,normal,0.2));
        glm::vec3 c = glm::vec3(0,1,0);
        float dot = glm::dot(vec,c);
        float angle = std::acosf(dot);
        c = a + c;
        glm::mat3 mat({
            a.x,a.y,1.0,
            b.x,b.y,1.0,
            c.x,c.y,1.0,
        });
        if(glm::determinant(mat) < 0){
            angle =-angle;
        }
        A->SetPosition(a - vec*(normal/2.0f));
        A->SetRotation(glm::vec3({0.0f,0.0f, angle * (180.0/glm::pi<float>()) }));
        projet->GetScene()->AddObjectScene(A);
        lstLines.push_back(A);
    }
}



void LineRenderer::Update(){

    glm::mat4* MVP = projet->getAppState()->GetMVP();
    if(MVP == nullptr){
        return;
    }
    glm::vec2 Mouse = projet->getAppState()->GetMousePosition();
    float width = projet->getAppState()->GetWidth();
    float height = projet->getAppState()->GetHeight();
    Mouse.x = ((Mouse.x*2.0f)-width)/width;
    Mouse.y = ((Mouse.y*2.0f)-height)/height;
    glm::mat4 mat = MVP[1]*MVP[0];
    mat = glm::inverse(mat);
    glm::vec4 vector({Mouse.x,Mouse.y,0.0f,1.0f});
    glm::vec4 pos = mat*vector;
    
    // pos.x = cos(time) * 10.0*(time/10.0);
    // pos.y = sin(time) * 10.0*(time/10.0);
    // pos.z = 0.0f;
    // time += 0.01;

    if(DelaunayTriangulation){
        CreateLineTriangle(trianglesPoints);
    }
    pos.w = 1.0 / pos.w;
    pos.x *= pos.w;
    pos.y *= pos.w;
    pos.z = 0.0f;

    ImGuiIO& io = ImGui::GetIO();

    if(ImGui::IsMouseClicked(0) && projet->getAppState()->GetGameViewHovered() && !supression){
        
        
        targetMouse = new Object(*projet->GetCube());
        targetMouse->SetName("Point");
        projet->GetScene()->AddObjectScene(targetMouse);
        targetMouse->SetPosition(glm::vec3({pos.x,pos.y,pos.z}));
        targetMouse->SetScale(glm::vec3({.2f,.2f,.2f}));
        lstObject.push_back(targetMouse);
        std::cout << lstObject.size() << std::endl;
        if(DelaunayTriangulation){
            TriangulationDelaunay();
        }
    }

    if(Voronoi){
        this->RenderVoronoi();
    }
    else{
        if(lstLinesVornoi.size() > 0){
            projet->GetScene()->RemoveObjectScene(lstLinesVornoi);
            lstLinesVornoi.clear();
        }
    }

    if(ImGui::IsMouseClicked(0) && projet->getAppState()->GetGameViewHovered() && supression){
        SuppressionDelaunay(glm::vec3({pos.x,pos.y,pos.z}));
    }


    if(RenderLine){
        CreateLine();
    }
    else if(!RenderLine && lstLines.size() > 0 && !DelaunayTriangulation){
        projet->GetScene()->RemoveObjectScene(lstLines);
        lstLines.clear();
    }

    return;
}

void LineRenderer::CopyObject( std::vector<Object*> point){
   
    this->lstObject;
    this->lstLines;
    projet = this->obj->GetProjet();

    for(Object* o : point){
        Object* t = new Object(*o);
        lstObject.push_back(t);
        projet->GetScene()->AddObjectScene(t);
    }
}

