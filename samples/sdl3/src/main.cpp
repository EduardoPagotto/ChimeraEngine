#include "AssetManager.hpp"
#include "TextureCrt.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanTexture.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <glm/ext/matrix_transform.hpp>
#include <string>

void teste(ce::VulkanContext& context) {

    using namespace ce;

    // 1. Cria o Asset Manager injetando o contexto Vulkan
    AssetManager assetManager(context);
    // BindlessUniformSampler uniform(context);
    // uniform.create();

    // std::string fileLoc = "./assets/textures/" + filePath;

    // 2. Carrega textura no Escopo Global
    auto logo = assetManager.load<VulkanTexture>("logo_jogos", "./assets/textures/grid1.png");

    {
        // 3. Entra em uma nova fase (Cria novo escopo)
        assetManager.pushScope();

        // Carrega textura específica desta fase
        auto bg = assetManager.load<VulkanTexture>("background", "./assets/textures/grid2.png");

        // Se pedir a "logo_jogos" aqui, ele busca e encontra no escopo Global pai
        auto logoMenu = assetManager.load<VulkanTexture>("logo_jogos", "./assets/textures/grid3.png");

    } // 5. Sai da fase (PopScope deve ser chamado manualmente ou via RAII)
    assetManager.popScope();
    // Neste ponto, "fase1_bg.png" foi destruído do Vulkan automaticamente.
    // "logo.png" continua vivo no escopo global.
}

int main() {

    auto result = SDL_APP_SUCCESS;

    // Habilita todas as mensagens em modo Debug
    SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);

    try {

        ce::VulkanContext context{};
        context.createWindow("Teste z1");

        VulkanRenderer vulkanRenderer(context);

        float angle = 0.0F;
        float deltaTime = 0;
        float lastTime = 0;

        int helicopter = vulkanRenderer.createMeshModel("./assets/models/Seahawk.obj");

        teste(context);

        for (bool running = true; running;) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running = false;
                }
            }

            float now = SDL_GetTicks() / 1000.0F;
            deltaTime = now - lastTime;
            lastTime = now;

            angle += 10.0F * deltaTime;
            if (angle > 360.0F) {
                angle -= 360.0F;
            }

            glm::mat4 testMat = glm::rotate(glm::mat4(1.0F), glm::radians(angle), glm::vec3(0.0F, 1.0F, 0.0F));
            //  testMat = glm::rotate(testMat, glm::radians(-45.0F), glm::vec3(0.0F, 0.0F, 1.0F));
            //  this->modelList[0].setModel(testMat);

            vulkanRenderer.updateModel(helicopter, testMat);
            vulkanRenderer.draw();
        }

    } catch (const std::runtime_error& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
        result = SDL_APP_FAILURE;
    }

    SDL_Quit();

    return result;
}
