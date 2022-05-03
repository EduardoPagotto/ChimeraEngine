#include <cstdio>
#include <iostream>
#include <map>

#ifdef OVR_SET_TO_USE
#include "chimera/render/CanvasOVR.hpp"
#else
#include "chimera/core/device/CanvasGL.hpp"
#endif
#include "Game.hpp"

int main(int argn, char** argv) {
    using namespace Chimera;
    try {
        // SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_DEBUG);
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("AppShader Iniciado");

        // Canvas* canvas = new CanvasGL("models", 800, 600);
        // Scene scene;

        Engine engine(new CanvasGL("models", 800, 600));
        Game* game = new Game(&engine);

        engine.pushState(game);
        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
        delete game;

        SDL_Log("AppShader finalizado com sucesso");
        return 0;

    } catch (const std::exception& ex) {
        // Fail 2
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::string& ex) {
        // Fail 3
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // Fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}
