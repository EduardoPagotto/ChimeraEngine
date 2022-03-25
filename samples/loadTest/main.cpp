#include "chimera/colladaDB/colladaLoad.hpp"
#include <SDL2/SDL.h>

int main(int argn, char** argv) {
    using namespace Chimera;

    Registry r;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("LoadTest Iniciado");

    colladaLoad(r, "./assets/models/piso2_mestre.xml");

    SDL_Log("LoadTest finalizado com sucesso");
    return 0;
}
