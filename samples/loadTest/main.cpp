#include "chimera/colladaDB/loader.hpp"
#include <SDL2/SDL.h>
#include <cstdio>

int main(int argn, char** argv) {
    using namespace Chimera;

    Registry r;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("LoadTest Iniciado");

    InstanceCollada* handle = colladaURL(nullptr, "visual_scene", "file://./assets/models/piso2_mestre.xml#Scene");
    // InstanceCollada* handle = colladaURL(nullptr, "image", "file://./assets/models/piso2_mestre.xml#teste1_png");
    // InstanceCollada* i = colladaURL(handle, "image", "teste1_png");
    // loadImage(handle, "teste1_png");

    loadAll(handle, &r);

    SDL_Log("LoadTest finalizado com sucesso");
    return 0;
}
