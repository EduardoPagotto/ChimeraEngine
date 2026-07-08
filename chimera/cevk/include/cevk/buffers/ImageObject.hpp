#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

    class ImageObject {
      public:
        explicit ImageObject(VkPhysicalDevice physical, VkDevice device) : physical(physical), device(device) {}
        virtual ~ImageObject();

        ImageObject(const ImageObject&) = delete;
        ImageObject& operator=(const ImageObject&) = delete;

        void createImage(uint32_t with, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags,
                         VkMemoryPropertyFlags propFlags);

        void createImageViewImportedImage(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

        void createImageView(VkImageAspectFlags aspectFlags);

        VkImageView& getImageView() { return this->imageView; }
        VkImage& getImage() { return this->image; }
        VkDeviceMemory& getImageMemory() { return this->imageMemory; }

      private:
        void destroy();

        bool isImported{false};
        VkFormat format;
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice device{VK_NULL_HANDLE};
        VkImageView imageView{VK_NULL_HANDLE};
        VkImage image{VK_NULL_HANDLE};
        VkDeviceMemory imageMemory{VK_NULL_HANDLE};
    }; // namespace ce
} // namespace ce
