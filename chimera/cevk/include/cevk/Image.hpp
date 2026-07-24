#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {
    class Image {
      public:
        explicit Image() = default;
        explicit Image(VkPhysicalDevice physical, VkDevice device) : physical(physical), device(device) {}
        virtual ~Image();

        void init(VkPhysicalDevice physical, VkDevice device) {
            this->physical = physical;
            this->device = device;
        }

        void destroy();

        void createImage(uint32_t with, uint32_t height, VkFormat format, VkImageTiling tiling,
                         VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags);

        void createImageViewImportedImage(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

        void createImageView(VkImageAspectFlags aspectFlags);

        VkImageView& getImageView() { return this->imageView; }
        VkImage& getImage() { return this->image; }
        VkDeviceMemory& getImageMemory() { return this->imageMemory; }

      private:
        bool isImported{false};
        VkFormat format;
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice device{VK_NULL_HANDLE};
        VkImageView imageView{VK_NULL_HANDLE};
        VkImage image{VK_NULL_HANDLE};
        VkDeviceMemory imageMemory{VK_NULL_HANDLE};
    };
} // namespace ce
