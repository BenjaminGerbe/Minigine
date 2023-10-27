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

        for(Component* c: copy){
                if(c->GetID() == c_Transform){
                        Transform& tr = *dynamic_cast<Transform*>(c);
                        Transform* nTr = new Transform(tr);
                        components.push_back(nTr);
                }
                else if(c->GetID() == c_Light){
                        LightComp& tr = *dynamic_cast<LightComp*>(c);
                        LightComp* nTr = new LightComp(tr);
                        components.push_back(nTr);
                }
                else if(c->GetID() == c_LineRenderer){
                        LineRenderer& tr = *dynamic_cast<LineRenderer*>(c);
                        LineRenderer* nTr = new LineRenderer(tr);
                        components.push_back(nTr);
                }
                else{
                       components.push_back( new Component(*c));
                }
        }
}

void Object::AddComponents(){
        if(objectType == Loader)
                return;

        this->components.push_back(new Transform(this));

        if(objectType == Light){
                this->components.push_back(new LightComp(this));
        }

        if(objectType == Camera){
               this->components.push_back(new CameraComp(this));
        }

        if(objectType == o_LineRenderer){
                this->components.push_back(new LineRenderer(this));
        }
}

void Object::SetObjectType(ObjectType type){
        this->objectType = type;
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