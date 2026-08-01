#include "VulkanRenderer.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include <string>

int main() {

    auto result = SDL_APP_SUCCESS;

    // Habilita todas as mensagens em modo Debug
    SDL_SetLogPriority(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_DEBUG);

    try {

        std::shared_ptr<ce::VulkanContext> ctx = std::make_shared<ce::VulkanContext>();

        ctx->createWindow("Teste z1");

        VulkanRenderer vulkanRenderer(ctx);

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
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
        result = SDL_APP_FAILURE;
    }

    SDL_Quit();

    return result;
}
