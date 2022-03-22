#include "chimera/colladaDB/Collada.hpp"
#include "chimera/colladaDB/ColladaNode.hpp"
#include <SDL2/SDL.h>
#include <cstdio>

int main(int argn, char** argv) {
    using namespace Chimera;

    Registry r;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("LoadTest Iniciado");

    Collada cl;
    pugi::xml_node vs = cl.urlRoot(pugi::xml_node(), "library_visual_scenes", "file://./assets/models/piso2_mestre.xml#Scene");

    ColladaNode cn;
    cn.loadAll(vs, &r);

    SDL_Log("LoadTest finalizado com sucesso");
    return 0;
}
