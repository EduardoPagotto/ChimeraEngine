#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {
    class Image {
      public:
        explicit Image() = default;
        explicit Image(VkPhysicalDevice physical, VkDevice device) : physical_(physical), device_(device) {}
        virtual ~Image();

        void init(VkPhysicalDevice physical, VkDevice device) {
            physical_ = physical;
            device_ = device;
        }

        void destroy();

        void createImage(uint32_t with, uint32_t height, VkFormat format, VkImageTiling tiling,
                         VkImageUsageFlags use_flags, VkMemoryPropertyFlags prop_flags);

        void createImageViewImportedImage(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags);

        void createImageView(VkImageAspectFlags aspect_flags);

        VkImageView& getImageView() { return image_view_; }
        VkImage& getImage() { return image_; }
        VkDeviceMemory& getImageMemory() { return image_memory_; }

      private:
        bool is_imported_{false};
        VkFormat format_;
        VkPhysicalDevice physical_{VK_NULL_HANDLE};
        VkDevice device_{VK_NULL_HANDLE};
        VkImageView image_view_{VK_NULL_HANDLE};
        VkImage image_{VK_NULL_HANDLE};
        VkDeviceMemory image_memory_{VK_NULL_HANDLE};
    };
} // namespace ce
