#include "../Headers/DisplayerManager.h"

bool isToDelete(RenderContextDisplay *element){
    return !element->GetOpen();
}

void DisplayerManager::RenderAllRenderWindows(int width,int height,Scene* scene){

    std::vector<RenderContextDisplay*>::iterator it = RenderContextDisplays.begin();
    int idx=0;
    while(it != RenderContextDisplays.end()){
        if( isToDelete(*it)){
            delete *it;
            it = RenderContextDisplays.erase(it);
        }
        else{
            char buffer[50]; 
            char* str = "Scene View ";
            sprintf(buffer, "%s%d", str,idx);
            RenderContextDisplays[idx]->DisplayRenderWindow(width,height,scene,buffer);
            idx++;
            it++;
        }
    }

    
}