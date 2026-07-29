#define STB_IMAGE_IMPLEMENTATION

#include "VulkanRenderer.hpp"
#include <SDL3/SDL.h>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <string>

void teste() {

    // // 1. Inicializa Vulkan e SDL3
    // VulkanContext context{};
    // // ... preencher struct context com os handles reais da sua engine ...

    // // 2. Cria o Asset Manager injetando o contexto Vulkan
    // AssetManager assetManager(context);

    // // 3. Carrega textura no Escopo Global
    // auto logo = assetManager.Load<VulkanTexture>("logo_jogos", "assets/logo.png");

    // {
    //     // 4. Entra em uma nova fase (Cria novo escopo)
    //     assetManager.PushScope();

    //     // Carrega textura específica desta fase
    //     auto bg = assetManager.Load<VulkanTexture>("background", "assets/fase1_bg.png");

    //     // Se pedir a "logo_jogos" aqui, ele busca e encontra no escopo Global pai
    //     auto logoMenu = assetManager.Load<VulkanTexture>("logo_jogos", "assets/logo.png");

    // } // 5. Sai da fase (PopScope deve ser chamado manualmente ou via RAII)
    // assetManager.PopScope();
    // // Neste ponto, "fase1_bg.png" foi destruído do Vulkan automaticamente.
    // // "logo.png" continua vivo no escopo global.

    // return 0;
}

int main() {

    auto result = SDL_APP_SUCCESS;

    try {

        ce::VulkanContext context{};
        context.createWindow("Teste z1");

        VulkanRenderer vulkanRenderer(context);

        float angle = 0.0F;
        float deltaTime = 0;
        float lastTime = 0;

        int helicopter = vulkanRenderer.createMeshModel("./assets/models/Seahawk.obj");

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
        std::cout << "Error: " << e.what() << '\n';
        result = SDL_APP_FAILURE;
    }

    SDL_Quit();

    return result;
}
