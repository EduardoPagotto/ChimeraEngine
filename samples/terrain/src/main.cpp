#include "Game.hpp"
#include "chimera/core/device/CanvasGL.hpp"

int main(int argn, char** argv) {
    using namespace Chimera;
    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Iniciado");

        Engine engine(new CanvasGL("Chimera", 1200, 600));
        Game* game = new Game(&engine);

        engine.pushState(game);
        engine.run();
        delete game;

        SDL_Log("Finalizado com sucesso");
        return 0;

    } catch (const std::string& ex) {
        // fail 3
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // Fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}
