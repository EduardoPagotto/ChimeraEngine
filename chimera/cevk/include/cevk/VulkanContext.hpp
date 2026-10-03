#pragma once

#include "cevk.hpp"
#include <array>
#include <string>

namespace ce {

    class VulkanContext {
      public:
        VulkanContext() = default;
        ~VulkanContext() { destroy(); }

        void create_window(const std::string& name = "Teste", const int& width = 800, const int& height = 600);
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

        static uint32_t find_memory_type_index(VkPhysicalDevice physical_device, uint32_t allowed_types,
                                               VkMemoryPropertyFlags properties);

        static SwapChainDetails get_swap_chain_details(VkPhysicalDevice device, VkSurfaceKHR surface);

        static VkFormat choose_supported_format(VkPhysicalDevice device, const std::vector<VkFormat>& formats,
                                                VkImageTiling tilling, VkFormatFeatureFlags feature_flags);

      private:
        VkDebugReportCallbackEXT callback_;
        bool validation_enabled_ = true;

        static constexpr std::array<const char*, 1> device_extensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        static constexpr std::array<const char*, 1> validation_layers{"VK_LAYER_KHRONOS_validation"};

        void create_instance();
        void create_debug_callback();
        void create_surface();
        void get_new_physical_device();
        void create_logical_device();
        void create_graphics_pool();

        // utils
        static QueueFamilyIndices get_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);
        static bool check_device_extension_support(VkPhysicalDevice device);
        static bool check_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        static bool check_instance_extension_support(std::vector<const char*>* check_extentions);
        static bool check_validation_layer_support();
        static bool check_descriptor_indexing_support(VkPhysicalDevice device);
    };
} // namespace ce
