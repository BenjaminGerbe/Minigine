#include "../Headers/Projet.h"
#include "../Headers/Transform.h"
#include "../Headers/Camera.h"
#include "../Headers/LineRenderer.h"
#include "../Headers/Light.h"

void Projet::SetUpComponents(){
    Components.push_back(new CameraComp(objects[5]));
    Components.push_back(new LightComp(objects[5]));
    Components.push_back(new LineRenderer(objects[5]));
}

void Projet::SetUpDefaultObject(){
    Object* Empty = new Object(defaultPrimitive[0],"Empty",ClassicObject);
    Object* Cube = new Object(defaultPrimitive[1],"cube",ClassicObject);
    Object* Dragon = new Object(defaultPrimitive[2],"dragon",ClassicObject);
    Object* oLight = new Object(defaultPrimitive[0],"light",Light);
    Object* oCamera = new Object(defaultPrimitive[0],"camera",Camera);
    Object* LineRenderer = new Object(defaultPrimitive[0],"LineRenderer",o_LineRenderer);

    objects.push_back(Empty);
    objects.push_back(Cube);
    objects.push_back(Dragon);
    objects.push_back(oLight);
    objects.push_back(oCamera);
    objects.push_back(LineRenderer);

}