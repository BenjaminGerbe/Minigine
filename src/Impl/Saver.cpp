#include "../Headers/Saver.h"

void Saver::SaveScene(Scene* scene){

    YAML::Node yamlFile;
    yamlFile["Objects"] = scene->GetObjects().size(); 
    for (int i = 0; i < scene->GetObjects().size(); i++)
    {
        Object* obj = scene->GetObjects()[i];
        std::string id = std::to_string(i);
        std::string name = obj->GetStrName();
        yamlFile[id]["ObjectName"] = name;
        yamlFile[id]["ObjectType"] = (int)obj->GetObjectType();
        yamlFile[id]["MeshFileID"] = obj->GetMesh()->GetID();
        
        yamlFile[id]["Color"].push_back(obj->GetColor().x);
        yamlFile[id]["Color"].push_back(obj->GetColor().y);
        yamlFile[id]["Color"].push_back(obj->GetColor().z);
        
        
        yamlFile[id]["ComponentSize"] = obj->GetComponents().size();
        for (int i = 0; i < obj->GetComponents().size(); i++)
        {
            Component* Component = obj->GetComponents()[i];
            Component->Save(id,i,yamlFile);
        }


    }  
    std::ofstream MyFile("scene.yaml");
    MyFile  <<  yamlFile;
    MyFile.close();
}

Mesh* FindMesh(Projet* projet,uint32_t fileID){
    bool find = false;
    int i = 0;
    Mesh* mesh;

    while(!find && i< projet->GetObjs().size()){
        mesh = projet->GetObjs()[i]->GetMesh();

        if(mesh->GetID() == fileID){
            find = true;
        }
        else{
            i++;
        }
    }

    return mesh;
}

void Saver::LoadScene(Projet* projet){
    Scene* scene = projet->GetScene();
    Scene* s = new Scene();
    projet->SetScene(s);
    YAML::Node yamlFile = YAML::LoadFile("scene.yaml");

    int size = yamlFile["Objects"].as<int>();
    
    for (int i = 0; i < size; i++)
    {
        std::string id = std::to_string( i);

        std::string name = yamlFile[id]["ObjectName"].as<std::string>();
    
        uint32_t fileNameID = yamlFile[id]["MeshFileID"].as<uint32_t>();
        Mesh* mesh = FindMesh(projet,fileNameID);
           
        ObjectType type =  (ObjectType)yamlFile[id]["ObjectType"].as<int>(); 
        Object* tempObj = new Object(mesh,name,Loader);
        int componentSize =  yamlFile[id]["ComponentSize"].as<int>();

        ImVec4 col = ImVec4(1.0f,1.0f,1.0f,1.0f);
        col.x = yamlFile[id]["Color"][0].as<float>();
        col.y = yamlFile[id]["Color"][1].as<float>();
        col.z = yamlFile[id]["Color"][2].as<float>();

        tempObj->SetColor(col);
        
        for (int j = 0; j < componentSize; j++)
        {
            ComponentID ID = (ComponentID)yamlFile[id][j]["ID"].as<int>();
            
            if(ID == c_Transform){
                tempObj->AddComponent(new Transform(id,j,yamlFile,tempObj));
            }
            else if(ID == c_Light){
                tempObj->AddComponent(new LightComp(id,j,yamlFile,tempObj));
            }
            else if(ID == c_Camera){
                tempObj->AddComponent(new CameraComp(id,j,yamlFile,tempObj));
            }
            else if(ID == c_LineRenderer){
                tempObj->AddComponent(new LineRenderer(id,j,yamlFile,tempObj));
            }
        }
        
        tempObj->SetObjectType(type);
        tempObj->SetProjet(projet);
        s->AddObjectScene(tempObj);
        tempObj->SetUp();
    }
    
    s->SetUp();
 
    delete scene;
}