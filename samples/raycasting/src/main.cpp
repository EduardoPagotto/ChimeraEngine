#include "Game.hpp"
#include "chimera/core/CanvasFB.hpp"
#include "chimera/core/Exception.hpp"
#include <iostream>

int main(int argn, char** argv) {
    using namespace Chimera;
    try {

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Simple ray-casting Iniciado");

        Engine engine(new CanvasFB("RayCasting", 640, 480));

        Game* game = new Game(&engine);

        engine.pushState(game);
        engine.run();

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
