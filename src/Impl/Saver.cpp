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
        yamlFile[id]["Transformation"]["Position"].push_back(obj->GetPosition().x);
        yamlFile[id]["Transformation"]["Position"].push_back(obj->GetPosition().y);
        yamlFile[id]["Transformation"]["Position"].push_back(obj->GetPosition().z);

        yamlFile[id]["Transformation"]["Rotation"].push_back(obj->GetRotation().x);
        yamlFile[id]["Transformation"]["Rotation"].push_back(obj->GetRotation().y);
        yamlFile[id]["Transformation"]["Rotation"].push_back(obj->GetRotation().z);

        yamlFile[id]["Transformation"]["Scale"].push_back(obj->GetScale().x);
        yamlFile[id]["Transformation"]["Scale"].push_back(obj->GetScale().y);
        yamlFile[id]["Transformation"]["Scale"].push_back(obj->GetScale().z);
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
    YAML::Node yamlFile = YAML::LoadFile("scene.yaml");

    int size = yamlFile["Objects"].as<int>();
    
    for (int i = 0; i < size; i++)
    {
        std::string id = std::to_string( i);

        std::string name = yamlFile[id]["ObjectName"].as<std::string>();
    
        uint32_t fileNameID = yamlFile[id]["MeshFileID"].as<uint32_t>();
        Mesh* mesh = FindMesh(projet,fileNameID);

        double x = yamlFile[id]["Transformation"]["Position"][0].as<double>();
        double y = yamlFile[id]["Transformation"]["Position"][1].as<double>();
        double z = yamlFile[id]["Transformation"]["Position"][2].as<double>();

        glm::vec3 position({x,y,z});

        x = yamlFile[id]["Transformation"]["Rotation"][0].as<double>();
        y = yamlFile[id]["Transformation"]["Rotation"][1].as<double>();
        z = yamlFile[id]["Transformation"]["Rotation"][2].as<double>();

        glm::vec3 rotation({x,y,z});

        x = yamlFile[id]["Transformation"]["Scale"][0].as<double>();
        y = yamlFile[id]["Transformation"]["Scale"][1].as<double>();
        z = yamlFile[id]["Transformation"]["Scale"][2].as<double>();


        ObjectType type =  (ObjectType)yamlFile[id]["ObjectType"].as<int>(); 

        glm::vec3 scale({x,y,z});

        Object* tempObj = new Object(mesh,name,type);
        tempObj->SetPosition(position);
        tempObj->SetRotation(rotation);
        tempObj->SetScale(scale);

        s->AddObjectScene(tempObj);
    }
    
    s->SetUp();
    projet->SetScene(s);
    delete scene;
}