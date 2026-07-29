#pragma once
#include "cevk.hpp"
#include <filesystem>

namespace ce {

    class DevVk {
      public:
        explicit DevVk(VulkanContext& context) : context(context) { this->init_device(); }
        virtual ~DevVk();

      private:
        VkDebugReportCallbackEXT callback;

        VulkanContext& context;

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
        static bool checkDescriptorIndexingSupport(VkPhysicalDevice device);
    };

    namespace aux {

        std::vector<char> readFile(const std::filesystem::path& filename);
    } // namespace aux

} // namespace ce
