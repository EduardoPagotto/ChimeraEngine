#include "Image.hpp"
#include "DevVK.hpp"
#include <stdexcept>

namespace ce {
    Image::~Image() { this->destroy(); }

    void Image::createImage(uint32_t with, uint32_t height, VkFormat format, VkImageTiling tiling,
                            VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags) {
        // CREATE IMAGE
        // Image Create Info
        const VkImageCreateInfo imageCreateInfo{
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
            .usage = useFlags,                          // Bit flags defined what image will be usage for
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,   // Whether image cam be shared between queues
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED, // Layout of image data on creation
        };

        if (vkCreateImage(this->device, &imageCreateInfo, nullptr, &this->image) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create an image!");
        }

        this->format = format;
        // CREATE MEMORY FOR IMAGE

        // Get Memory requirement for a type of image
        VkMemoryRequirements memoryRequirements;
        vkGetImageMemoryRequirements(this->device, this->image, &memoryRequirements);

        // Allocate memory using image requeirement and user define properties
        const VkMemoryAllocateInfo memoryAllocInfo{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = aux::FindMemoryTypeIndex(this->physical, memoryRequirements.memoryTypeBits, propFlags)};

        if (vkAllocateMemory(this->device, &memoryAllocInfo, nullptr, &this->imageMemory) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Allocate Memory for Image");
        }

        // Connect memory to image
        vkBindImageMemory(this->device, this->image, this->imageMemory, 0);

        this->isImported = false;
    }

    void Image::createImageViewImportedImage(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
        this->image = image;
        this->format = format;
        this->isImported = true;
        this->createImageView(aspectFlags);
    }

    void Image::createImageView(VkImageAspectFlags aspectFlags) {
        //
        const VkImageViewCreateInfo viewCreateInfo{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,  //
            .image = this->image,                               // Image to create view for
            .viewType = VK_IMAGE_VIEW_TYPE_2D,                  // Type of image (1D, 2D, 3D, Cube, etc)
            .format = this->format,                             // Format of image data
            .components = {.r = VK_COMPONENT_SWIZZLE_IDENTITY,  // Allows remapping of rgba component to other values
                           .g = VK_COMPONENT_SWIZZLE_IDENTITY,  //
                           .b = VK_COMPONENT_SWIZZLE_IDENTITY,  //
                           .a = VK_COMPONENT_SWIZZLE_IDENTITY}, //
            .subresourceRange = {
                // Subresources allow the view to view only a part of a image
                .aspectMask = aspectFlags, // which aspect of image to view (e.g. COLOR_BIT for view color)
                .baseMipLevel = 0,         // Start mipmap level to start from
                .levelCount = 1,           // Number of mipmap levels to view
                .baseArrayLayer = 0,       // Start array level to view from
                .layerCount = 1            // Numbers of array levels to view
            }};

        // Create image view and return it
        if (vkCreateImageView(this->device, &viewCreateInfo, nullptr, &this->imageView) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create an Image View!");
        }
    }

    void Image::destroy() {
        if (device == VK_NULL_HANDLE) {
            return;
        }

        if (this->imageView != VK_NULL_HANDLE) {
            vkDestroyImageView(this->device, this->imageView, nullptr);
            this->imageView = VK_NULL_HANDLE;
        }

        if ((this->image != VK_NULL_HANDLE) && (!this->isImported)) {
            vkDestroyImage(this->device, this->image, nullptr);
            this->image = {VK_NULL_HANDLE};
        }

        if (this->imageMemory != VK_NULL_HANDLE) {
            vkFreeMemory(this->device, this->imageMemory, nullptr);
            this->imageMemory = VK_NULL_HANDLE;
        }

        device = VK_NULL_HANDLE;
    }
} // namespace ce
