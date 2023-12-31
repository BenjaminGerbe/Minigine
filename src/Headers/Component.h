#pragma once
#include "Object.h"
#include "yaml-cpp/yaml.h"

enum ComponentID{
    c_None,
    c_Transform,
    c_Light,
    c_Camera,
    c_LineRenderer,
    c_Mesh,
    c_Material,
    c_Water,
    c_LineRenderer3D
};

class Component
{
    protected:
    Object* obj;
    ComponentID ID;
    bool open;
    char* headerName;

    virtual void SetID(){
        ID = c_None;
    }
    private:


    public:
    Component()=default;

    Component(Object* _obj):obj(_obj){
        SetID();
        open = true;
        headerName = "component";
    }

    Component(const Component& copy){
        this->obj = copy.obj;
        this->ID = copy.ID;
        this->open = copy.open;
        this->headerName = copy.headerName;
    }

    virtual Component* Clone(Object* obj){
        Component* comp = new Component(obj);
        return comp;
    }

    Component(std::string id,int i,YAML::Node& yamlFile,Object* obj){
        this->obj = obj;
        this->ID = (ComponentID)yamlFile[id][i]["ID"].as<int>();
        this->open = (ComponentID)yamlFile[id][i]["open"].as<bool>();

        headerName = "component";
    }

    Component operator=(const Component& copy){
        this->obj = copy.obj;
        this->ID = copy.ID;
        this->open = copy.open;
        this->headerName = copy.headerName;

        return *this;
    }

    virtual void Save(std::string id,int i,YAML::Node& yamlFile){
        std::string header(headerName);
        yamlFile[id][i]["ID"] = (int)ID;
        yamlFile[id][i]["open"] = open;
        yamlFile[id][i]["headerName"] = header;
    }


    char* GetHeaderName(){
        return headerName;
    }

    bool GetOpen(){
        return open;
    }

    void SetOpen(bool b){
        open = b;
    }

    Object* GetObj(){
        return obj;
    }

    void SetObj(Object* obj){
        this->obj = obj;
    }

    virtual void Editor();

    virtual void SetUp();

    virtual void Update();

    ComponentID GetID(){
        return this->ID;
    }

    virtual ~Component(){
    }
};

