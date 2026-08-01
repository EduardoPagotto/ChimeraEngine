#include "Game.hpp"
#include "cevk/TextureBindless.hpp"
#include "cevk_engine/AssetManager.hpp"
#include "cevk_engine/ScreenVK.hpp"
#include "cevk_infra/Engine.hpp"
#include <SDL3/SDL.h>
#include <memory>

void teste(std::shared_ptr<ce::VulkanContext> ctx) {

    using namespace ce;

    // 1. Cria o Asset Manager injetando o contexto Vulkan
    AssetManager assetManager(ctx);
    TextureBindless texBindless(ctx);

    // Sampler sampler(ctx->logical);

    // std::string fileLoc = "./assets/textures/" + filePath;

    // 2. Carrega textura no Escopo Global
    auto logo = assetManager.load<VulkanTexture>("logo_jogos", "./assets/textures/grid1.png");

    texBindless.allocTexture(logo);

    {
        // 3. Entra em uma nova fase (Cria novo escopo)
        assetManager.pushScope();

        // Carrega textura específica desta fase
        auto bg = assetManager.load<VulkanTexture>("background", "./assets/textures/grid2.png");

        texBindless.allocTexture(bg);

        // Se pedir a "logo_jogos" aqui, ele busca e encontra no escopo Global pai
        auto logoMenu = assetManager.load<VulkanTexture>("logo_jogos", "./assets/textures/grid3.png");

    } // 5. Sai da fase (PopScope deve ser chamado manualmente ou via RAII)
    assetManager.popScope();
    // Neste ponto, "fase1_bg.png" foi destruído do Vulkan automaticamente.
    // "logo.png" continua vivo no escopo global.
}

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

        teste(ctx);

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
