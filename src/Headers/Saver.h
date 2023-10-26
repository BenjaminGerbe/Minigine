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

namespace Saver{
    void SaveScene(Scene* scene);
    void LoadScene(Projet* projet);
}

