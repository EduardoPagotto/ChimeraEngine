#include "Game.hpp"
#include "chimera/core/Exception.hpp"
#include "chimera/render/CanvasGL.hpp"
#include <iostream>

int main(int argn, char** argv) {
    using namespace Chimera;
    try {

        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Iniciado");

        Engine engine(new CanvasGL("simples", 640, 480));
        Game* game = new Game(&engine);

        engine.pushState(game);
        engine.run();

        delete game;

        SDL_Log("Sucesso");
        return 0;

    } catch (const Chimera::Exception& ex) {
        // fail 1
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::exception& ex) {
        // fali 2
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.what());
    } catch (const std::string& ex) {
        // fail 3
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha grave: %s", ex.c_str());
    } catch (...) {
        // Fail 4
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
    }

    return -1;
}
