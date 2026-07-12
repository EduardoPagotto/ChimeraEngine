#pragma once
#include "cevk.hpp"
#include <filesystem>

namespace ce {

    class DevVk {
      public:
        explicit DevVk(std::shared_ptr<BaseVK> bvk) : bvk(bvk) { init_device(); } // NOLINT
        virtual ~DevVk();

        std::shared_ptr<BaseVK> getBaseVK() const { return bvk; }

        [[nodiscard]] VkQueue& getGraphicsQueue() { return graphicsQueue; }
        [[nodiscard]] VkQueue& getPresentationQueue() { return presentationQueue; }

      private:
        // Vulkan components
        // - Main
        VkInstance instance;
        VkDebugReportCallbackEXT callback;
        VkQueue graphicsQueue;
        VkQueue presentationQueue;
        std::shared_ptr<BaseVK> bvk;

        bool validationEnabled = true;

        inline static std::vector<const char*> deviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        inline static std::vector<const char*> validationLayers{"VK_LAYER_KHRONOS_validation"};

        // - Create functions
        void init_device();
        void createInstance();
        void createDebugCallback();
        void createSurface();
        void getNewPhysicalDevice();
        void createLogicalDevice();

        // utils
        static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        static bool CheckDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        static bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtentions);
        static bool CheckValidationLayerSupport();
    };

    namespace aux {

        VkFormat ChooseSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& formats,
                                       VkImageTiling tilling, VkFormatFeatureFlags featureFlags);

        SwapChainDetails GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

        QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

        // -- Swapchain
        uint32_t FindMemoryTypeIndex(VkPhysicalDevice physicalDevice, uint32_t allowedTypes,
                                     VkMemoryPropertyFlags properties);

        std::vector<char> readFile(const std::filesystem::path& filename);
    } // namespace aux

} // namespace ce
