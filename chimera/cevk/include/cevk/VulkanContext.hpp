#pragma once

#include "cevk.hpp"

namespace ce {

    class VulkanContext {
      public:
        VulkanContext() = default;
        ~VulkanContext() { this->destroy(); }

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

        static SwapChainDetails GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

        static VkFormat ChooseSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& formats,
                                              VkImageTiling tilling, VkFormatFeatureFlags featureFlags);

      private:
        VkDebugReportCallbackEXT callback;
        bool validationEnabled = true;

        inline static std::vector<const char*> deviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        inline static std::vector<const char*> validationLayers{"VK_LAYER_KHRONOS_validation"};

        void createInstance();
        void createDebugCallback();
        void createSurface();
        void getNewPhysicalDevice();
        void createLogicalDevice();
        void createGraphicsPool();

        // utils
        static QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
        static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        static bool CheckDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        static bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtentions);
        static bool CheckValidationLayerSupport();
        static bool checkDescriptorIndexingSupport(VkPhysicalDevice device);
    };
} // namespace ce
