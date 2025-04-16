#include "Game.hpp"
#include "chimera/core/Engine.hpp"
#include "chimera/core/device/CanvasFB.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include <iostream>

int main(int argn, char** argv) {
    using namespace ce;
    try {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
        SDL_Log("Simple ray-casting Iniciado");

        // Registry to entt
        // auto reg = std::make_shared<Registry>();

        // Services shared inside all parts
        // Canvas, Mouse, keyboard, Joystick, gamepad, view's
        g_service_locator.registerService(std::make_shared<Keyboard>());
        g_service_locator.registerService(std::make_shared<CanvasFB>("BSP Tree", 800, 600, false));
        g_service_locator.registerService(std::make_shared<ViewProjection>()); // not used but necessary

        // Engine
        Engine engine;

        Game* game = new Game;

        engine.getStack().pushState(game);
        engine.run();

        SDL_Log("Loop de Game encerrado!!!!");
        delete game;
        SDL_Log("raycasting finalizado com sucesso");
        return 0;

    } catch (const std::string& ex) { SDL_Log("Falha grave: %s", ex.c_str()); } catch (...) {
        SDL_Log("Falha Desconhecida");
    }

    return -1;
}
