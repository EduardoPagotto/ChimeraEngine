#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#ifdef SET_GLFW_ENABLE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#else
#include <SDL3/SDL_vulkan.h>
#endif
#include <vulkan/vulkan_core.h>

namespace ce {

    struct BaseVK {
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice logical{VK_NULL_HANDLE};
        VkSurfaceKHR surface{VK_NULL_HANDLE};
#ifdef SET_GLFW_ENABLE
        GLFWwindow* window{nullptr};
#else
        SDL_Window* window{nullptr};
#endif
    };
} // namespace ce
