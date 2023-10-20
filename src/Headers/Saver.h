#pragma once
#include "yaml-cpp/yaml.h"
#include "Scene.h"
#include <fstream>
#include "Projet.h"
namespace Saver{
    void SaveScene(Scene* scene);
    void LoadScene(Projet* projet);
}

