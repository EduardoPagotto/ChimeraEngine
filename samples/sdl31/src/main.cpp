#include "Game.hpp"
#include "cevk_engine/ScreenVK.hpp"
#include "cevk_infra/Engine.hpp"
#include <SDL3/SDL.h>
#include <memory>

int main(int argc, char* argv[]) {

    auto result = SDL_APP_SUCCESS;

    // Habilita todas as mensagens em modo Debug
    SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);

    try {

        std::shared_ptr<ce::VulkanContext> ctx = std::make_shared<ce::VulkanContext>();

        ctx->createWindow("Teste z1");

        std::shared_ptr<ce::ScreenVK> scr = std::make_shared<ce::ScreenVK>(ctx);

        ce::Engine engine(scr);
        std::shared_ptr<ce::IStateMachine> game = std::make_shared<Game>(ctx, scr);

        engine.getStack().pushState(game);
        engine.run();

        SDL_Log("Finalizado");

    } catch (const std::runtime_error& e) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
        result = SDL_APP_FAILURE;

    } catch (...) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
        result = SDL_APP_FAILURE;
    }

    return result;
}
