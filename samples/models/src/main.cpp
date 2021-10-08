#include <cstdio>
#include <iostream>
#include <map>

#ifdef OVR_SET_TO_USE
#include "chimera/core/windows/CanvasOVR.hpp"
#else
#include "chimera/core/windows/CanvasGL.hpp"
#endif
#include "Game.hpp"
#include "chimera/core/Exception.hpp"

int main(int argn, char** argv) {
    try {
        // SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_DEBUG);
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("AppShader Iniciado");

        Game* game = new Game(new Chimera::CanvasGL("models", 800, 600));
        game->run();

        SDL_Log("Loop de Game encerrado!!!!");
        delete game;

        SDL_Log("AppShader finalizado com sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // Fail 1
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
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
