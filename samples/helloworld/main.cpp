#include "Game.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/device/CanvasGL.hpp"
#include <iostream>

int main(int argn, char** argv) {

    using namespace Chimera;
    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Models3 Iniciado");

        RegistryManager::create();

        Engine engine(new CanvasGL("BSP Tree", 800, 600, false), 0.0f);

        ColladaDom dom = loadFileCollada("./samples/helloworld/level.xml");
        colladaRegistryLoad(dom, RegistryManager::get());

        engine.init();

        Game* game = new Game(engine);

        Collada::destroy(); // clean loader

        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
        delete game;

        SDL_Log("AppShader finalizado com sucesso");
        return 0;

    } catch (const std::exception& ex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.what()); // Exception generica
    } catch (const std::string& ex) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro: %s", ex.c_str()); // Exception string
    } catch (...) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro Desconhecida"); // Exception geral
    }

    return -1;
}
