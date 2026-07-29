#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    const int MAX_FRAME_DRAWS = 2;
    const int MAX_OBJECTS = 30;

    //
    // Vertex data representation
    struct Vertex {
        glm::vec3 pos; // Vertex Position (x, y, z)
        glm::vec3 col; // Vertex Color (r, g, b)
        glm::vec2 tex; // Texture Coords (u, v)
    };

    struct SwapChainDetails {
        VkSurfaceCapabilitiesKHR surfaceCapabilities;    // Surface properties, e.g. image size/extent
        std::vector<VkSurfaceFormatKHR> formats;         // Surface image formats, e.g. RGBA and size of each colour
        std::vector<VkPresentModeKHR> presentationModes; // How images should be presented to screen
    };

    struct QueueFamilyIndices {
        int graphicsFamily = -1;     // Location of graphics Queue Family
        int presentationFamily = -1; // Location of Presentation Queue family
        // check if queue families are valid
        [[nodiscard]] bool isValid() const { return (graphicsFamily >= 0) && (presentationFamily >= 0); }
    };

    class VulkanContext {
      public:
        VulkanContext() = default;
        ~VulkanContext() = default;
        void init();
        void destroy();

        VkInstance instance{VK_NULL_HANDLE};
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice logical{VK_NULL_HANDLE};
        VkSurfaceKHR surface{VK_NULL_HANDLE};
        VkCommandPool commandPool{VK_NULL_HANDLE};
        VkQueue graphicsQueue{VK_NULL_HANDLE};
        VkQueue presentationQueue{VK_NULL_HANDLE};
        QueueFamilyIndices queueFamilyIndices;
        SDL_Window* window{nullptr};

        static uint32_t FindMemoryTypeIndex(VkPhysicalDevice physicalDevice, uint32_t allowedTypes,
                                            VkMemoryPropertyFlags properties);

        static QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

        static SwapChainDetails GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

        static VkFormat ChooseSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& formats,
                                              VkImageTiling tilling, VkFormatFeatureFlags featureFlags);
    };

} // namespace ce
