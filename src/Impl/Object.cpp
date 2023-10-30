#include "../Headers/Object.h"
#include "../Headers/Component.h"
#include "../Headers/Transform.h"
#include "../Headers/Light.h"
#include "../Headers/Camera.h"
#include "../Headers/LineRenderer.h"


void Object::DeleteComponents(){
        for(auto p : this->components){
                delete p;
        }
}

void Object::SetUp(){
        for(Component* c: components){
                c->SetUp();
        }
}

void Object::CopyComponents(std::vector<Component*> copy){
        DeleteComponents();

        for (Component* c : copy) {
                Component* newComponent = c->Clone(this);
                components.push_back(newComponent);
        }
}

void Object::AddComponents(){
        this->color = ImVec4(0.85f,0.85f,0.85f,1.0f);
        if(objectType == Loader)
                return;

        this->components.push_back(new Transform(this));

        if(objectType == Light){
                this->components.push_back(new LightComp(this));
                this->color = ImVec4(0.91f,0.96f,0.25f,1.0f);
        }

        if(objectType == Camera){
               this->components.push_back(new CameraComp(this));
               this->color = ImVec4(0.18f,0.39f,0.96f,1.0f);
        }

        if(objectType == o_LineRenderer){
                this->components.push_back(new LineRenderer(this));
                this->color = ImVec4(0.96f,0.39f,0.18f,1.0f);
        }
}

void Object::SetObjectType(ObjectType type){
        this->objectType = type;
}

void Object::DeleteComponent(int i){
        Component* comp = components[i];
        components.erase(components.begin()+i);
        delete comp;
}

glm::mat4 Object::GetTransformation(){
        
        transformation = glm::mat4(1.0f);
        
        transformation *= glm::translate(glm::mat4(1.0f),position);
        transformation *= glm::rotate(glm::mat4(1.0f), rotation.x * (glm::pi<float>() / 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        transformation *= glm::rotate(glm::mat4(1.0f), rotation.y * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformation *= glm::rotate(glm::mat4(1.0f), rotation.z * (glm::pi<float>() / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transformation *= glm::scale(glm::mat4(1.0f),scale);  
      
        return transformation;
};