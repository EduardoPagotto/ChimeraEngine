#define STB_IMAGE_IMPLEMENTATION
#include "VulkanRenderer.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <string>

SDL_Window* window = nullptr;

bool initWindow(const std::string& sName = "Teste", const int width = 800, const int height = 600) {

    // 1. Initialize SDL3
    if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11")) {
        std::cerr << "SDL X11 Failed: " << SDL_GetError() << '\n';
        return false;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Failed: " << SDL_GetError() << '\n';
        return false;
    }

    // 2. Create Window with Vulkan support
    window = SDL_CreateWindow(sName.c_str(), width, height, SDL_WINDOW_VULKAN);
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << '\n';
        return false;
    }

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Vulkan SD3 Window Created");

    return true;
}

int main() {

    auto result = SDL_APP_SUCCESS;

    try {

        if (!initWindow("Teste")) {
            return SDL_APP_FAILURE;
        }

        std::shared_ptr<ce::BaseVK> bvk = std::make_shared<ce::BaseVK>();
        bvk->window = window;

        ce::DevVk devvk(bvk);
        VulkanRenderer vulkanRenderer(devvk);

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

    SDL_DestroyWindow(window);
    SDL_Quit();

    return result;
}
