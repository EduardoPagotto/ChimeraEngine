#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

    VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

    void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                       const VkAllocationCallbacks* pAllocator);

    VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,        // Type of error
                                                 VkDebugReportObjectTypeEXT objType, // Type of object causing error
                                                 uint64_t obj,                       // ID of object
                                                 size_t location, int32_t code, const char* layerPrefix,
                                                 const char* message, // Validation Information
                                                 void* userData);
} // namespace ce
