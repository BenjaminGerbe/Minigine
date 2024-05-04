#pragma once
#include "yaml-cpp/yaml.h"
#include "Scene.h"
#include <fstream>
#include "Projet.h"
#include "Component.h"
#include "Transform.h"
#include "Light.h"
#include "Camera.h"
#include "LineRenderer.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "WaterComponent.h"
#include "LineRenderer3D.h"
#include <functional>
#include <string>

class Factory{
	public :
	std::map<const std::string, void* (*)()> map;

    void* Create(const std::string& id) {
        auto it = map.find(id);
        if (it != map.end()) {
            return it->second();
        }
        return nullptr;
    }

    template<class Base>
    void Register( const RTTI<Base>& rtii, void*(*creator)()) {
        map[rtii.type] = creator;
    }

};

namespace Saver{
    static Factory factory;
    void static GenerateFactory(){
        factory.Register(
            Component::rtti,
           []() -> void* {
            std::cout << "creation" << std::endl;
            return nullptr; 
        });

        factory.Create(Component::rtti.type);
    }

    void SaveScene(Scene* scene);
    void LoadScene(Projet* projet);
}

