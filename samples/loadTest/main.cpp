#include "chimera/colladaDB/loader.hpp"
#include <SDL2/SDL.h>
#include <cstdio>

int main(int argn, char** argv) {
    // using namespace Chimera;
    //  try {
    //  SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_DEBUG);
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("LoadTest Iniciado");

    Chimera::LoadColladaURL loader("file://./assets/models/piso2_mestre.xml#scene");

    // Engine engine(new CanvasGL("models", 800, 600));
    // Game* game = new Game(&engine);

    // engine.pushState(game);
    // engine.run();

    // SDL_Log("Loop de Game encerrado!!!!");
    // delete game;

    SDL_Log("LoadTest finalizado com sucesso");
    return 0;
}
