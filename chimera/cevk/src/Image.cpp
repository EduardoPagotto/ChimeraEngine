#include "Image.hpp"
#include "VulkanContext.hpp"
#include <stdexcept>

namespace ce {
    Image::~Image() { destroy(); }

    void Image::create_image(uint32_t with, uint32_t height, VkFormat format, VkImageTiling tiling,
                             VkImageUsageFlags use_flags, VkMemoryPropertyFlags prop_flags) {
        // CREATE IMAGE
        // Image Create Info
        const VkImageCreateInfo image_create_info{
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType = VK_IMAGE_TYPE_2D,              // Type of image (1D, 2D or 3D)
            .format = format,                           // Format type of image
            .extent = {.width = with,                   // Width of image extent
                       .height = height,                // Height of image extent
                       .depth = 1},                     // Depth of image (just 1, no 3D aspect)
            .mipLevels = 1,                             // Number of mipmaps levels
            .arrayLayers = 1,                           // Number of levels in image array
            .samples = VK_SAMPLE_COUNT_1_BIT,           // Number of samples for multi-sampling
            .tiling = tiling,                           // How image data shoud be "tiled" (arranged for optima reading)
            .usage = use_flags,                         // Bit flags defined what image will be usage for
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,   // Whether image cam be shared between queues
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED, // Layout of image data on creation
        };

        if (vkCreateImage(device_, &image_create_info, nullptr, &image_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create an image!");
        }

        format_ = format;
        // CREATE MEMORY FOR IMAGE

        // Get Memory requirement for a type of image
        VkMemoryRequirements memory_requirements;
        vkGetImageMemoryRequirements(device_, image_, &memory_requirements);

        // Allocate memory using image requeirement and user define properties
        const VkMemoryAllocateInfo memory_alloc_info{.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                                                     .allocationSize = memory_requirements.size,
                                                     .memoryTypeIndex = VulkanContext::find_memory_type_index(
                                                         physical_, memory_requirements.memoryTypeBits, prop_flags)};

        if (vkAllocateMemory(device_, &memory_alloc_info, nullptr, &image_memory_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Allocate Memory for Image");
        }

        // Connect memory to image
        vkBindImageMemory(device_, image_, image_memory_, 0);

        is_imported_ = false;
    }

    void Image::create_image_view_imported_image(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags) {
        image_ = image;
        format_ = format;
        is_imported_ = true;
        create_image_view(aspect_flags);
    }

    void Image::create_image_view(VkImageAspectFlags aspect_flags) {
        //
        const VkImageViewCreateInfo view_create_info{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,  //
            .image = image_,                                    // Image to create view for
            .viewType = VK_IMAGE_VIEW_TYPE_2D,                  // Type of image (1D, 2D, 3D, Cube, etc)
            .format = format_,                                  // Format of image data
            .components = {.r = VK_COMPONENT_SWIZZLE_IDENTITY,  // Allows remapping of rgba component to other values
                           .g = VK_COMPONENT_SWIZZLE_IDENTITY,  //
                           .b = VK_COMPONENT_SWIZZLE_IDENTITY,  //
                           .a = VK_COMPONENT_SWIZZLE_IDENTITY}, //
            .subresourceRange = {
                // Subresources allow the view to view only a part of a image
                .aspectMask = aspect_flags, // which aspect of image to view (e.g. COLOR_BIT for view color)
                .baseMipLevel = 0,          // Start mipmap level to start from
                .levelCount = 1,            // Number of mipmap levels to view
                .baseArrayLayer = 0,        // Start array level to view from
                .layerCount = 1             // Numbers of array levels to view
            }};

        // Create image view and return it
        if (vkCreateImageView(device_, &view_create_info, nullptr, &image_view_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create an Image View!");
        }
    }

    void Image::destroy() {
        if (device_ == VK_NULL_HANDLE) {
            return;
        }

        if (image_view_ != VK_NULL_HANDLE) {
            vkDestroyImageView(device_, image_view_, nullptr);
            image_view_ = VK_NULL_HANDLE;
        }

        if ((image_ != VK_NULL_HANDLE) && (!is_imported_)) {
            vkDestroyImage(device_, image_, nullptr);
            image_ = {VK_NULL_HANDLE};
        }

        if (image_memory_ != VK_NULL_HANDLE) {
            vkFreeMemory(device_, image_memory_, nullptr);
            image_memory_ = VK_NULL_HANDLE;
        }

        device_ = VK_NULL_HANDLE;
    }
} // namespace ce
