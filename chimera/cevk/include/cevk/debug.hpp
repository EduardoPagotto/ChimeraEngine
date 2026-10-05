#pragma once

#include <vulkan/vulkan_core.h>

namespace ce {

    VkResult create_debug_report_callback_ext(VkInstance instance,
                                              const VkDebugReportCallbackCreateInfoEXT* p_create_info,
                                              const VkAllocationCallbacks* p_allocator,
                                              VkDebugReportCallbackEXT* p_callback);

    void destroy_debug_report_callback_ext(VkInstance instance, VkDebugReportCallbackEXT callback,
                                           const VkAllocationCallbacks* p_allocator);

    VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugReportFlagsEXT flags,         // Type of error
                                                  VkDebugReportObjectTypeEXT obj_type, // Type of object causing error
                                                  uint64_t obj,                        // ID of object
                                                  size_t location, int32_t code, const char* layer_prefix,
                                                  const char* message, // Validation Information
                                                  void* user_data);
} // namespace ce
