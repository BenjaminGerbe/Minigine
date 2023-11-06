#include "../Headers/LineRenderer.h"
#include "../Headers/Projet.h"

struct PolyPoint{
    int idx;
    float angle;
    float length;
    glm::vec3 position;
};

struct Segment{
    glm::vec3 A;
    glm::vec3 B;
};

void LineRenderer::Triangulation(){
    if(lstObject.size() <= 2)
        return;

    float xmin = lstObject[0]->GetPosition().x;
    float ymin = lstObject[0]->GetPosition().y;
    float epsilon = std::numeric_limits<float>::epsilon();
    int size = lstObject.size();
    std::vector<Object*> points(lstObject);
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
    
    std::vector<Segment> segments;
    std::vector<Segment> segmentsToRender;
    for (int i = 0; i < 3; i++)
    {
        int nxt = (i+1)%3;
        int prec = (i-1);
        if(prec < 0){
            prec=2;
        }

        glm::vec3 precV = points[prec]->GetPosition();
        glm::vec3 nxtV = points[nxt]->GetPosition();
        glm::vec3 currentV = points[i]->GetPosition();

        glm::mat3 m({
        precV.x,precV.y,1.0,
        currentV.x,currentV.y,1.0,
        nxtV.x,nxtV.y,1.0,
        });

        Segment tmp;

        if(glm::determinant(m) < 0){
            tmp.A = points[nxt]->GetPosition();
            tmp.B = points[i]->GetPosition();
        }
        else{
            tmp.A = points[i]->GetPosition();
            tmp.B = points[nxt]->GetPosition();
        }

        segments.push_back(tmp);
        segmentsToRender.push_back(tmp);
    }
    
    points.erase(points.begin()+0);
    points.erase(points.begin()+0);
    points.erase(points.begin()+0);
    
    for (int i = 0; i < points.size(); i++)
    {   
        glm::vec3 position = points[i]->GetPosition();
        std::vector<int> segmentsToAdd;
        for (int j = 0; j < segments.size(); j++)
        {
            glm::vec3 p1 = segments[j].A;
            glm::vec3 p2 = segments[j].B;
            glm::vec3 d = p2-p1;
            glm::vec3 normal(-d.y,d.x,p1.z);

            glm::mat3 mat({
            p1.x,p1.y,1.0,
            p2.x,p2.y,1.0,
            position.x,position.y,1.0,
            });

            glm::vec3 dir = glm::normalize(p1 - position);
            if(glm::determinant(mat) < 0){
              //  normal = -normal;
                std::cout << "clockWize" << std::endl;
            }
            normal = glm::normalize(normal);
            if(glm::dot(normal,dir) > 0){
                segmentsToAdd.push_back(j);
            }
        }
        
       for (int k = 0; k < segmentsToAdd.size(); k++)
       {
            Segment seg;
            Segment seg2;
            glm::vec3 precV = segments[segmentsToAdd[k]].A;
            glm::vec3 nxtV = points[i]->GetPosition();
            glm::vec3 currentV = segments[segmentsToAdd[k]].B;

            glm::mat3 m({
            precV.x,precV.y,1.0,
            currentV.x,currentV.y,1.0,
            nxtV.x,nxtV.y,1.0,
            });

            if(glm::determinant(m) < 0){
                seg.A = precV;
                seg.B = nxtV;
                seg2.A =nxtV;
                seg2.B = currentV;
            }
            else{
                seg.A = nxtV;
                seg.B =  precV;
                seg2.A = currentV;
                seg2.B = nxtV;
            }

            bool find1 = false;
            bool find2 = false;
            int idx1 = -1;
            int idx2 = -1;
            for (int l = 0; l < segments.size(); l++)
            {
                Segment s = segments[l];
                if( glm::length(s.A-seg.A) < epsilon && glm::length( s.B-seg.B) < epsilon ||
                glm::length(s.B-seg.A) < epsilon && glm::length( s.A-seg.B) < epsilon ){
                    find1 = true;
                    idx1 = l;
                }

                if( glm::length(s.A-seg2.A) < epsilon && glm::length( s.B-seg2.B) < epsilon ||
                glm::length(s.B-seg2.A) < epsilon && glm::length( s.A-seg2.B) < epsilon ){
                    find2 = true;
                    idx2 = l;
                }
            }

            
            if(!find1){
                segments.push_back(seg);
                segmentsToRender.push_back(seg);
            }
            else{
                segments.erase(segments.begin() + idx1);
            }

            if(!find2){
                segments.push_back(seg2);
                segmentsToRender.push_back(seg2);
            }
            else{
                segments.erase(segments.begin() + idx2);
            }
           
       }

        for (int k = segmentsToAdd.size()-1; k >= 0; k--)
        {
            segments.erase(segments.begin()+segmentsToAdd[k]);
        }
    }
    
    
    for (int i = 0; i < segmentsToRender.size(); i++)
    {
        glm::vec3 a = segmentsToRender[i].A;
        glm::vec3 b = segmentsToRender[i].B;

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
       
    return;
}

void LineRenderer::SetUp(){
    projet = this->obj->GetProjet();
    lastIdx = 0;
    start = true;
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

