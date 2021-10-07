#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/windows/CanvasFB.hpp"
#include <iostream>

int main(int argn, char** argv) {

    try {

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Simple ray-casting Iniciado");

        Game* game = new Game(new Chimera::CanvasFB("RayCasting", 640, 480));
        game->run();
        SDL_Log("Loop de Game encerrado!!!!");
        delete game;
        SDL_Log("raycasting finalizado com sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) { SDL_Log("Falha grave: %s", ex.what()); } catch (const std::exception& ex) {
        SDL_Log("Falha grave: %s", ex.what());
    } catch (const std::string& ex) { SDL_Log("Falha grave: %s", ex.c_str()); } catch (...) {
        SDL_Log("Falha Desconhecida");
    }

    return -1;
}
