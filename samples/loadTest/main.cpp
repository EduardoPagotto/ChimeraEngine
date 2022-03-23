#include "chimera/colladaDB/Collada.hpp"
#include "chimera/colladaDB/ColladaPhysicScene.hpp"
#include "chimera/colladaDB/ColladaVisualScene.hpp"
#include <SDL2/SDL.h>
#include <cstdio>

int main(int argn, char** argv) {
    using namespace Chimera;

    Registry r;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("LoadTest Iniciado");

    Collada cl;
    pugi::xml_node vs = cl.urlRoot(pugi::xml_node(), "library_visual_scenes", "file://./assets/models/piso2_mestre.xml#Scene");

    ColladaVisualScene cn;
    cn.loadAll(vs, &r);

    pugi::xml_node fs = cl.urlRoot(pugi::xml_node(), "library_physics_scenes", "file://./assets/models/piso2_mestre.xml#Scene-Physics");
    ColladaPhysicScene cs;
    cs.loadAll(fs, &r);

    SDL_Log("LoadTest finalizado com sucesso");
    return 0;
}
