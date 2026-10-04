#include "Game.hpp"
#include "cevk_engine/CanvaVK.hpp"
#include "chimera_base/Engine.hpp"
#include <SDL3/SDL.h>
#include <memory>

int main(int argc, char* argv[]) {

    using namespace ce;

    auto result = EXIT_SUCCESS;

    try {
        // Habilita todas as mensagens em modo Debug
        SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);

        SDL_Log("SDL31 Iniciado");
        // Registry to entt
        std::shared_ptr<entt::registry> registry = std::make_shared<entt::registry>();
        registry->ctx().emplace<std::shared_ptr<ICanva>>(std::make_shared<CanvaVK>("Teste SDL31", 800, 600));
        registry->ctx().emplace<std::shared_ptr<InputManager>>(std::make_shared<InputManager>());
        // ctx->createWindow("Teste SDL31");

        Engine engine(registry);
        std::shared_ptr<IStateMachine> game = std::make_shared<Game>(registry);

        engine.stack().push_state(game);
        engine.run();

        SDL_Log("Finalizado");

    } catch (const std::runtime_error& e) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
        result = EXIT_FAILURE;

    } catch (...) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
        result = EXIT_FAILURE;
    }

    return result;
}
