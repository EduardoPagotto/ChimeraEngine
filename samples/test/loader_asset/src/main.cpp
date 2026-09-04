#include "cevk_infra/AssetManager.hpp"
#include "cevk_infra/Loader.hpp"

int main() {

    using namespace ce;

    auto result = EXIT_SUCCESS;

    try {

        SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);

        std::shared_ptr<VulkanContext> ctx = std::make_shared<VulkanContext>();
        ctx->createWindow("Teste z1");

        entt::registry registry;
        auto& assetManager = registry.ctx().emplace<AssetManager>();

        Loader loader("./assets/seahawk/Seahawk.gltf", &registry);
        loader.getImages("./assets/seahawk/", ctx);

        for (auto&& [id, handle] : assetManager.texture) {
            const VulkanTexture tex = *handle;
            SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "index: %d", tex.getIndex());
        }

        auto meshes = loader.getMeshs(ctx);

        assetManager.texture.clear();

    } catch (const std::runtime_error& e) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
        result = EXIT_FAILURE;

    } catch (...) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha Desconhecida");
        result = EXIT_FAILURE;
    }

    return result;
}
