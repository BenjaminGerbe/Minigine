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
        std::cout<<" P = (" << position.x << ","<<position.y << " )"<< std::endl;
        for (int j = (int)lstTriangles.size()-1; j >= 0 ; j--)
        {
            Triangle triangle = *lstTriangles[j];
            for (int k = 0; k < 3; k++)
            {
                Segment* seg = triangle.GetSegment(k);
                std::cout << "A_{"<<k << "} = (" << seg->A.x << ","<<seg->A.y << " )" <<std::endl; 
                std::cout << "B_{"<<k << "}  = (" << seg->B.x << ","<<seg->B.y << " )" <<std::endl; 
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

    for(auto p:garbage){
        delete p;
    }

    for(auto p:lstTriangles){
        delete p;
    }

    return;
 }

// void LineRenderer::Triangulation(){
//     if(lstObject.size() <= 2)
//         return;

//     float xmin = lstObject[0]->GetPosition().x;
//     float ymin = lstObject[0]->GetPosition().y;
//     float epsilon = std::numeric_limits<float>::epsilon();
//     int size = lstObject.size();
//     std::vector<Object*> points(lstObject);
//     bool finish = false;

//     while(!finish){
//         finish = true;
//         for (int i = 0; i < lstObject.size()-1; i++)
//         {
//             glm::vec3 pt0 = points[i]->GetPosition();
//             glm::vec3 pt1 = points[i+1]->GetPosition();
            
//             if(pt1.x < pt0.x || ( std::abs( pt1.x - pt0.x) < epsilon && pt1.y < pt0.y)){
//                 std::swap(points[i],points[i+1]);
//                 finish = false;
//             }
            
//         }
//     }
    
//     std::vector<Segment> segments;
//     std::vector<Segment> segmentsToRender;
//     std::vector<Triangle> lstTriangle;
//     Triangle trgl;
//     for (int i = 0; i < 3; i++)
//     {
//         int nxt = (i+1)%3;
//         int prec = (i-1);
//         if(prec < 0){
//             prec=2;
//         }

//         glm::vec3 precV = points[prec]->GetPosition();
//         glm::vec3 nxtV = points[nxt]->GetPosition();
//         glm::vec3 currentV = points[i]->GetPosition();

//         glm::mat3 m({
//         precV.x,precV.y,1.0,
//         currentV.x,currentV.y,1.0,
//         nxtV.x,nxtV.y,1.0,
//         });

//         Segment tmp;

//         if(glm::determinant(m) < 0){
//             tmp.A = points[nxt]->GetPosition();
//             tmp.B = points[i]->GetPosition();
//         }
//         else{
//             tmp.A = points[i]->GetPosition();
//             tmp.B = points[nxt]->GetPosition();
//         }

//         segments.push_back(tmp);
//         segmentsToRender.push_back(tmp);

//         trgl(i,tmp);
//     }
    
//     Triangle tr;
//     tr.A = trgl.C;
//     tr.B = trgl.B;
//     tr.C = trgl.A;

//     points.erase(points.begin()+0);
//     points.erase(points.begin()+0);
//     points.erase(points.begin()+0);
    
//     lstTriangle.push_back(tr);


//     for (int i = 0; i < points.size(); i++)
//     {   
//         glm::vec3 position = points[i]->GetPosition();
//         std::vector<int> segmentsToAdd;
//         for (int j = 0; j < segments.size(); j++)
//         {
//             glm::vec3 p1 = segments[j].A;
//             glm::vec3 p2 = segments[j].B;
//             glm::vec3 d = p2-p1;
//             glm::vec3 normal(-d.y,d.x,p1.z);

//             glm::mat3 mat({
//             p1.x,p1.y,1.0,
//             p2.x,p2.y,1.0,
//             position.x,position.y,1.0,
//             });

//             glm::vec3 dir = glm::normalize(p1 - position);
//             if(glm::determinant(mat) < 0){
//               //  normal = -normal;
//                 std::cout << "clockWize" << std::endl;
//             }

//             normal = glm::normalize(normal);
//             if(glm::dot(normal,dir) > 0){
//                 segmentsToAdd.push_back(j);
//             }
//         }
        


//        for (int k = 0; k < segmentsToAdd.size(); k++)
//        {
//             Segment seg;
//             Segment seg2;
//             glm::vec3 precV = segments[segmentsToAdd[k]].A;
//             glm::vec3 nxtV = points[i]->GetPosition();
//             glm::vec3 currentV = segments[segmentsToAdd[k]].B;

//             glm::mat3 m({
//             precV.x,precV.y,1.0,
//             currentV.x,currentV.y,1.0,
//             nxtV.x,nxtV.y,1.0,
//             });

//             if(glm::determinant(m) < 0){
//                 seg.A = precV;
//                 seg.B = nxtV;
//                 seg2.A =nxtV;
//                 seg2.B = currentV;
//             }
//             else{
//                 seg.A = nxtV;
//                 seg.B =  precV;
//                 seg2.A = currentV;
//                 seg2.B = nxtV;
//             }

//             bool find1 = false;
//             bool find2 = false;

//             for (int l = segments.size()-1; l >=0 ; l--)
//             {
//                 Segment s = segments[l];
//                 if( glm::length(s.A-seg.A) < epsilon && glm::length( s.B-seg.B) < epsilon ||
//                 glm::length(s.B-seg.A) < epsilon && glm::length( s.A-seg.B) < epsilon ){
//                     find1 = true;
//                     segments.erase(segments.begin() + l);
//                 }

//                 if( glm::length(s.A-seg2.A) < epsilon && glm::length( s.B-seg2.B) < epsilon ||
//                 glm::length(s.B-seg2.A) < epsilon && glm::length( s.A-seg2.B) < epsilon ){
//                     find2 = true;
//                     segments.erase(segments.begin() + l);
//                 }
//             }

//             Triangle trgle;
//             trgle.A = segments[segmentsToAdd[k]];   
//             trgle.B = seg;
//             trgle.C = seg2;  
//             lstTriangle.push_back(trgle);

//             if(!find1){
//                 segments.push_back(seg);
//             }
           
//             if(!find2){
//                 segments.push_back(seg2);
//             }
           
//        }

//         for (int k = segmentsToAdd.size()-1; k >= 0; k--)
//         {
//             segments.erase(segments.begin()+segmentsToAdd[k]);
//         }
//     }

//     if(Delaunay){
//         for (int i = 0; i < lstTriangle.size()-1; i++)
//             {
//                 Triangle gl1 = lstTriangle[i];
//                 Triangle gl2 = lstTriangle[i+1];
//                 glm::vec3 A = gl1[1].B;
//                 glm::vec3 B = gl1[2].B;
//                 glm::vec3 C = gl1[1].A;
//                 glm::vec3 D = gl2[2].A;

//                 glm::mat4 m({
//                     A.x,A.y,std::pow(A.x,2)+std::pow(A.y,2),1,
//                     B.x,B.y,std::pow(B.x,2)+std::pow(B.y,2),1,
//                     C.x,C.y,std::pow(C.x,2)+std::pow(C.y,2),1,
//                     D.x,D.y,std::pow(D.x,2)+std::pow(D.y,2),1
//                 });

//                 if(glm::determinant(m) > 0){
//                     Triangle tr1;
//                     Triangle tr2;
//                     Segment seg;
//                     seg.A = A;
//                     seg.B = B;
//                     tr1.A = seg;
//                     seg.A = B;
//                     seg.B = D;
//                     tr1.B = seg;
//                     seg.A = D;
//                     seg.B = A;
//                     tr1.C = seg;

//                     seg.A = D;
//                     seg.B = B;
//                     tr2.A = seg;
//                     seg.A = B;
//                     seg.B =C;
//                     tr2.B = seg;
//                     seg.A = C;
//                     seg.B = D;
//                     tr2.C = seg;

                   
//                     lstTriangle[i] = tr1;
//                     lstTriangle[i+1] = tr2;
//                 }

//             }
  
//     }
    

//     for (int i = 0; i < lstTriangle.size(); i++)
//     {
//         for (int k = 0; k < 3; k++)
//         {
//             glm::vec3 a = lstTriangle[i][k].A;
//             glm::vec3 b = lstTriangle[i][k].B;

//             Object* A = new Object(*projet->GetCube());
//             A->SetName("Line");
//             glm::vec3 vec = a-b;
//             float normal = glm::length(vec);
//             vec = glm::normalize(vec);
//             A->SetScale(glm::vec3(0.2,normal,0.2));
//             glm::vec3 c = glm::vec3(0,1,0);
//             float dot = glm::dot(vec,c);
//             float angle = std::acosf(dot);
//             c = a + c;
//             glm::mat3 mat({
//                 a.x,a.y,1.0,
//                 b.x,b.y,1.0,
//                 c.x,c.y,1.0,
//             });
//             if(glm::determinant(mat) < 0){
//                 angle =-angle;
//             }
//             A->SetPosition(a - vec*(normal/2.0f));
//             A->SetRotation(glm::vec3({0.0f,0.0f, angle * (180.0/glm::pi<float>()) }));
//             projet->GetScene()->AddObjectScene(A);
//             SegmentTriangles.push_back(A);
//         }
        
//     }
    
//     return;
// }

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
    if(ImGui::Button("Gift Wrapping")){
        GiftWraping();
    }
    ImGui::Spacing();
    if(ImGui::Button("Graham-Scan")){
        GrahamScan();
    }
    if(ImGui::Button("Triangulation")){
        Triangulation();
    }
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
    
    pos.w = 1.0 / pos.w;
    pos.x *= pos.w;
    pos.y *= pos.w;
    pos.z = 0.0f;

    ImGuiIO& io = ImGui::GetIO();

    if(ImGui::IsMouseClicked(0) && projet->getAppState()->GetGameViewHovered()){
        targetMouse = new Object(*projet->GetCube());
        targetMouse->SetName("Point");
        projet->GetScene()->AddObjectScene(targetMouse);
        targetMouse->SetPosition(glm::vec3({pos.x,pos.y,pos.z}));
        targetMouse->SetScale(glm::vec3({.2f,.2f,.2f}));
        lstObject.push_back(targetMouse);
        std::cout << lstObject.size() << std::endl;
    }


    if(RenderLine){
        CreateLine();
    }
    else if(!RenderLine && lstLines.size() > 0){
        // projet->GetScene()->RemoveObjectScene(lstLines);
        // lstLines.clear();
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

