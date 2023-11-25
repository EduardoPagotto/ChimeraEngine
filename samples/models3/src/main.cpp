#include "Game.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/device/CanvasGL.hpp"
#include "chimera/render/collada/ColladaRender.hpp"
#include <cstdio>
#include <iostream>
#include <map>

int main(int argn, char** argv) {
    using namespace Chimera;
    try {
        // SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_DEBUG);
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Models3 Iniciado");

        RegistryManager::create();

        Engine engine(new CanvasGL("Teste Hello", 1800, 600, false), 0.0f);

        ColladaDom dom = loadFileCollada("./assets/models/nivel1.xml");
        colladaRegistryLoad(dom, RegistryManager::get());
        colladaRenderLoad(dom, RegistryManager::get());

        engine.init();

        Scene scene(RegistryManager::get());

        Game* game = new Game(scene);

        engine.getStack().pushState(&scene);
        engine.getStack().pushState(game);

        Collada::destroy(); // clean loader

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
