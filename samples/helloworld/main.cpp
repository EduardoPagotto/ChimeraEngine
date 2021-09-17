#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/core/io/FlowControl.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include <iostream>

int main(int argn, char** argv) {

    using namespace Chimera;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("Hello iniciado");

    for (int i = 0; i < argn; i++)
        SDL_Log("Parametros %d: %s", i, argv[i]);

    try {
        CanvasGL* video = new CanvasGL("Hello", 960, 540);

        Game* game = new Game(video);
        Core::FlowControl* pControle = new Chimera::Core::FlowControl(game);
        pControle->run();

        SDL_Log("Loop de Game encerrado!!!!");

        delete pControle;
        delete game;
        delete video;

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
