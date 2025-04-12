#include "Game.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/collada/colladaLoad.hpp"
#include "chimera/core/device/CanvasGL.hpp"
#include "chimera/core/visible/FontMng.hpp"
#include "chimera/core/visible/ShaderMng.hpp"
#include "chimera/render/collada/ColladaRender.hpp"
#include <cstdio>
#include <iostream>
#include <map>

int main(int argn, char** argv) {
    using namespace ce;
    try {
        // SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_PRIORITY_DEBUG);
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Models3 Iniciado");

        // Services shared inside all parts
        // Canvas, Mouse, keyboard, Joystick, gamepad, view's
        auto sl = std::make_shared<ServiceLocator>();
        sl->registerService(std::make_shared<Registry>());
        sl->registerService(std::make_shared<CanvasGL>("Teste Hello", 1800, 600, false));
        sl->registerService(std::make_shared<Mouse>());
        sl->registerService(std::make_shared<GameController>());
        sl->registerService(std::make_shared<ViewProjection>()); // View projection
        sl->registerService(std::make_shared<ShaderMng>());
        sl->registerService(std::make_shared<FontMng>());
        sl->registerService(std::make_shared<TextureMng>());

        Engine engine(sl);

        ColladaDom dom = loadFileCollada("./assets/models/nivel1.xml");
        colladaRegistryLoad(dom, sl);
        colladaRenderLoad(dom, sl);

        Scene scene(sl);
        Game game(sl, &scene);

        engine.getStack().pushState(&scene);
        engine.getStack().pushState(&game);

        Collada::destroy(); // clean loader

        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
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
