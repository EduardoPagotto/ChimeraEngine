#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/render/CanvasGL.hpp"
#include <iostream>

int main(int argn, char** argv) {

    using namespace Chimera;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("Hello iniciado");

    for (int i = 0; i < argn; i++)
        SDL_Log("Parametros %d: %s", i, argv[i]);

    try {

        Engine engine(new CanvasGL("Hello", 960, 540));
        Game* game = new Game(&engine);

        engine.pushLayer(game);
        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
        delete game;

        SDL_Log("Hello finalizado");
        return 0;

    } catch (const Exception& ex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.what()); // Exception Chimera
    } catch (const std::exception& ex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.what()); // Exception generica
    } catch (const std::string& ex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.c_str()); // Exception string
    } catch (...) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro Desconhecida"); // Exception geral
    }

    return -1;
}
