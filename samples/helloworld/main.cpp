#include "Game.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/device/CanvasGL.hpp"
#include "chimera/core/device/Mouse.hpp"
#include "chimera/core/visible/FontMng.hpp"
#include "chimera/core/visible/ShaderMng.hpp"
#include <iostream>

int main(int argn, char** argv) {

    using namespace Chimera;
    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Models3 Iniciado");

        // Registry to entt
        auto sl = std::make_shared<ServiceLocator>();
        sl->registerService(std::make_shared<Registry>());
        sl->registerService(std::make_shared<CanvasGL>("Simples", 800, 600, false));
        sl->registerService(std::make_shared<Mouse>());
        sl->registerService(std::make_shared<ViewProjection>(0.0f)); // View projection
        sl->registerService(std::make_shared<ShaderMng>());
        sl->registerService(std::make_shared<FontMng>());
        sl->registerService(std::make_shared<TextureManager>());

        Engine engine(sl);

        ColladaDom dom = loadFileCollada("./samples/helloworld/level.xml");
        colladaRegistryLoad(dom, sl);

        Game game(sl, &engine);

        engine.getStack().pushState(&game);

        Collada::destroy(); // clean loader

        engine.run();
        SDL_Log("Loop de Game encerrado!!!!");
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
