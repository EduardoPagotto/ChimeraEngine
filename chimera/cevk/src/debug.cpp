#include "debug.hpp"
#include <SDL3/SDL_log.h>

namespace ce {

    VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* p_create_info,
                                          const VkAllocationCallbacks* p_allocator,
                                          VkDebugReportCallbackEXT* p_callback) {
        // vkGetInstanceProcAddr returns a function pointer to the requested function in the requested instance
        // resulting function is cast as a function pointer with the header of "vkCreateDebugReportCallbackEXT"
        auto func =
            (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

        // If function was found, executre if with given data and return result, otherwise, return error
        if (func != nullptr) {
            return func(instance, p_create_info, p_allocator, p_callback);
        }
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                       const VkAllocationCallbacks* p_allocator) {
        // get function pointer to requested function, then cast to function pointer for vkDestroyDebugReportCallbackEXT
        auto func =
            (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

        // If function found, execute
        if (func != nullptr) {
            func(instance, callback, p_allocator);
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,         // Type of error
                                                 VkDebugReportObjectTypeEXT obj_type, // Type of object causing error
                                                 uint64_t obj,                        // ID of object
                                                 size_t location, int32_t code, const char* layer_prefix,
                                                 const char* message, // Validation Information
                                                 void* user_data) {
        // If validation ERROR, then output error and return failure
        if ((flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "%s", message);
            return VK_TRUE;
        }

        // If validation WARNING, then output warning and return okay
        if ((flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "%s", message);
            return VK_FALSE;
        }

        return VK_FALSE;
    }

} // namespace ce
