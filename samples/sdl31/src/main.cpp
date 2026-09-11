#include "Game.hpp"
#include "cevk_engine/CanvaVK.hpp"
#include "chimera_base/Engine.hpp"
#include <SDL3/SDL.h>
#include <memory>

int main(int argc, char* argv[]) {

    using namespace ce;

    auto result = EXIT_SUCCESS;

    // Habilita todas as mensagens em modo Debug
    SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_DEBUG);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    SDL_SetLogPriority(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG);

    try {
        std::shared_ptr<entt::registry> registry = std::make_shared<entt::registry>();

        using enum InputEnable;
        InputEnable in = Mouse | Keyboard;

        registry->ctx().emplace<std::shared_ptr<InputManager>>(std::make_shared<InputManager>(in, true));
        auto& ctx = registry->ctx().emplace<std::shared_ptr<VulkanContext>>(std::make_shared<VulkanContext>());

        ctx->createWindow("Teste SDL31");

        std::shared_ptr<ce::CanvaVK> canva = std::make_shared<CanvaVK>(ctx);

        Engine engine(registry, canva);
        std::shared_ptr<IStateMachine> game = std::make_shared<Game>(registry, canva);

        engine.getStack().pushState(game);
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
