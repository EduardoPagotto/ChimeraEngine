#include "cevk.hpp"
#include <stdexcept>

namespace ce {

    void VulkanContext::init() {

        // Get inidices of queue families from device
        this->queueFamilyIndices = GetQueueFamilies(physical, surface);

        vkGetDeviceQueue(logical, queueFamilyIndices.graphicsFamily, 0, &this->graphicsQueue);
        vkGetDeviceQueue(logical, queueFamilyIndices.presentationFamily, 0, &this->presentationQueue);

        const VkCommandPoolCreateInfo poolInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                               .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                                               .queueFamilyIndex =
                                                   static_cast<uint32_t>(queueFamilyIndices.graphicsFamily)};

        // Create a Graphics Queue Family Command Pool
        if (vkCreateCommandPool(logical, &poolInfo, nullptr, &this->commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Command Pool");
        }
    }

    void VulkanContext::destroy() {
        if (this->commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(this->logical, this->commandPool, nullptr);
            this->commandPool = VK_NULL_HANDLE;
        }

        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyDevice(logical, nullptr);

        vkDestroyInstance(instance, nullptr);
    }

    //-------
    uint32_t VulkanContext::FindMemoryTypeIndex(VkPhysicalDevice physicalDevice, uint32_t allowedTypes,
                                                VkMemoryPropertyFlags properties) {
        // get properties of physical device memory
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {

            // Index of memory type must match corresponding bit in allowedTypes and desired property bit flag are
            // part of memory type's property flags
            if ((allowedTypes & (1 << i)) &&
                (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) { // NOLINT
                // this memory type is valid, so return its index
                return i;
            }
        }

        throw std::runtime_error("Failed to find Memory!");
    }

    QueueFamilyIndices VulkanContext::GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {

        QueueFamilyIndices indices;

        // Get all Queue Family Property info for the given device
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);

        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

        // Go through each queue family and check if it has at least 1 of the requered types of queue
        int idx = 0;
        for (const auto& queueFamily : queueFamilyList) {

            // First check if queue has at least 1 queue in that family (could have no queue)
            // Queue cam be multiple types defined through bitfield. Need to bitwise AND with VK_QUEUE_*_BIT to
            // check if has requered type
            if ((queueFamily.queueCount > 0) && ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)) {
                indices.graphicsFamily = idx; // if queue family is valid then get index
            }

            // Check if Queue Family support presentation
            VkBool32 presentationSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, idx, surface,
                                                 &presentationSupport); // TODO: validar se result OK
            // check if queue is presentation type (can bo boyh graphics and presentation)
            if ((queueFamily.queueCount > 0) && (presentationSupport == VK_TRUE)) {
                indices.presentationFamily = idx;
            }

            // check if queue family indices are in valid state, stop searching if so
            if (indices.isValid()) {
                break;
            }

            idx++;
        }

        return indices;
    }

    SwapChainDetails VulkanContext::GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SwapChainDetails swapChainDetails;

        // -- CAPABILITIES --
        // Get the surface capabilities for the given surface on the given physical device
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainDetails.surfaceCapabilities);

        // -- FORMATS --
        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        // If formats returned, get list of formats
        if (formatCount != 0) {
            swapChainDetails.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainDetails.formats.data());
        }

        // -- PRESENTATION MODES --
        uint32_t presentationCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, nullptr);

        // If presentation modes returned, get list of presentation modes
        if (presentationCount != 0) {
            swapChainDetails.presentationModes.resize(presentationCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount,
                                                      swapChainDetails.presentationModes.data());
        }

        return swapChainDetails;
    }

    VkFormat VulkanContext::ChooseSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& formats,
                                                  VkImageTiling tilling, VkFormatFeatureFlags featureFlags) {

        // Loop through options and find compatible one
        for (VkFormat format : formats) {

            // Get properties for give format on this device
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(device, format, &properties);

            // Depending on tiling choice, nned to check for difference bit flag
            if (tilling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & featureFlags) == featureFlags) {
                //
                return format;
            }
            if (tilling == VK_IMAGE_TILING_OPTIMAL &&
                (properties.optimalTilingFeatures & featureFlags) == featureFlags) {
                //
                return format;
            }
        }

        throw std::runtime_error("Failed to find a matching format!");
    }
} // namespace ce
