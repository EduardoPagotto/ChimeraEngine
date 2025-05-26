#include "Game.hpp"
#include "chimera/base/Engine.hpp"
#include "chimera/collada/ColladaRender.hpp"
#include "chimera/collada/colladaLoad.hpp"
#include "chimera/core/gl/CanvasGL.hpp"
#include "chimera/core/gl/FontMng.hpp"
#include "chimera/core/gl/ShaderMng.hpp"
#include <config_params.hpp>
#include <cstdio>
#include <iostream>
#include <map>

int main(int argn, char** argv) {
    using namespace ce;
    try {
        SDL_SetAppMetadata(std::string(project_name).c_str(), std::string(project_version).c_str(),
                           "com.mechanical.engine");

        SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Models3 Iniciado");

        // Services shared inside all parts
        // Canvas, Mouse, keyboard, Joystick, gamepad, view's
        g_service_locator.registerService(std::make_shared<CanvasGL>("Teste Hello", 1800, 600, false));
        g_service_locator.registerService(std::make_shared<Mouse>());
        g_service_locator.registerService(std::make_shared<GamePad>());
        g_service_locator.registerService(std::make_shared<ViewProjection>()); // View projection
        g_service_locator.registerService(std::make_shared<ShaderMng>());
        g_service_locator.registerService(std::make_shared<FontMng>());
        g_service_locator.registerService(std::make_shared<TextureMng>());

        Engine engine;

        ColladaDom dom = loadFileCollada("./assets/models/nivel1.xml");
        colladaRegistryLoad(dom);
        colladaRenderLoad(dom);

        Scene scene;
        Game game(&scene);

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
