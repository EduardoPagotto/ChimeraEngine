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
        glm::vec3 pos;      // Vertex Position (x, y, z)
        glm::vec3 col{0.0}; // Vertex Color (r, g, b)
        glm::vec2 tex;      // Texture Coords (u, v)
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

} // namespace ce
