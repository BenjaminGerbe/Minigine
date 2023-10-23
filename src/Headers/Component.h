#pragma once
#include "Object.h"


enum ComponentID{
    c_None,
    c_Transform,
    c_Light
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

    Component operator=(const Component& copy){
        this->obj = copy.obj;
        this->ID = copy.ID;
        this->open = copy.open;
        this->headerName = copy.headerName;

        return *this;
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

    Object* GetObject(){
        return obj;
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

