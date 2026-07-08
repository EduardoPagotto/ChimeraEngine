#include "debug.hpp"
#include <iostream>

namespace ce {

    VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
        // vkGetInstanceProcAddr returns a function pointer to the requested function in the requested instance
        // resulting function is cast as a function pointer with the header of "vkCreateDebugReportCallbackEXT"
        auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

        // If function was found, executre if with given data and return result, otherwise, return error
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pCallback);
        }
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
        // get function pointer to requested function, then cast to function pointer for vkDestroyDebugReportCallbackEXT
        auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

        // If function found, execute
        if (func != nullptr) {
            func(instance, callback, pAllocator);
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,        // Type of error
                                                 VkDebugReportObjectTypeEXT objType, // Type of object causing error
                                                 uint64_t obj,                       // ID of object
                                                 size_t location, int32_t code, const char* layerPrefix,
                                                 const char* message, // Validation Information
                                                 void* userData) {
        // If validation ERROR, then output error and return failure
        if ((flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0) {
            std::cout << "VALIDATION ERROR: " << message << '\n';
            return VK_TRUE;
        }

        // If validation WARNING, then output warning and return okay
        if ((flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0) {
            std::cout << "VALIDATION WARNING: " << message << '\n';
            return VK_FALSE;
        }

        return VK_FALSE;
    }

} // namespace ce
