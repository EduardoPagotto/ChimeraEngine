#include "VulkanContext.hpp"
#include "debug.hpp"
#include <SDL3/SDL_vulkan.h>
#include <cstring>
#include <format>
#include <set>
#include <stdexcept>

namespace ce {

    void VulkanContext::create_window(const std::string& name, const int& width, const int& height) {

        // 1. Initialize SDL3
        if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland")) {
            throw std::runtime_error(std::format("SDL wayland Failed driver: {}", SDL_GetError()));
        }

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error(std::format("SDL Video Failed: {}", SDL_GetError()));
        }

        // 2. Create Window with Vulkan support
        this->window = SDL_CreateWindow(name.c_str(), width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
        if (this->window == nullptr) {
            throw std::runtime_error(std::format("SDL Window creation failed: {}", SDL_GetError()));
        }

        SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Vulkan SDL3 Window Created OK");

        init();
    }

    void VulkanContext::init() {

        create_instance();
        create_debug_callback();
        create_surface();
        get_new_physical_device();
        create_logical_device();
        create_graphics_pool();
    }

    void VulkanContext::destroy() {
        if (this->commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(this->logical, this->commandPool, nullptr);
            this->commandPool = VK_NULL_HANDLE;
        }

        vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
        vkDestroyDevice(this->logical, nullptr);

        if (validation_enabled_) {
            DestroyDebugReportCallbackEXT(this->instance, this->callback_, nullptr);
        }

        vkDestroyInstance(this->instance, nullptr);

        SDL_DestroyWindow(this->window);
    }

#pragma region statics
    //-------
    uint32_t VulkanContext::find_memory_type_index(VkPhysicalDevice physical_device, uint32_t allowed_types,
                                                   VkMemoryPropertyFlags properties) {
        // get properties of physical device memory
        VkPhysicalDeviceMemoryProperties memory_properties;
        vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);

        for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {

            // Index of memory type must match corresponding bit in allowedTypes and desired property bit flag are
            // part of memory type's property flags
            if (((allowed_types & (1 << i)) > 0) &&
                (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
                // this memory type is valid, so return its index
                return i;
            }
        }

        throw std::runtime_error("Failed to find Memory!");
    }

    QueueFamilyIndices VulkanContext::get_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) {

        QueueFamilyIndices indices;

        // Get all Queue Family Property info for the given device
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
        std::vector<VkQueueFamilyProperties> queue_family_list(queue_family_count);

        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_family_list.data());

        // Go through each queue family and check if it has at least 1 of the requered types of queue
        int idx = 0;
        for (const auto& queue_family : queue_family_list) {

            // First check if queue has at least 1 queue in that family (could have no queue)
            // Queue cam be multiple types defined through bitfield. Need to bitwise AND with VK_QUEUE_*_BIT to
            // check if has requered type
            if ((queue_family.queueCount > 0) && ((queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)) {
                indices.graphicsFamily = idx; // if queue family is valid then get index
            }

            // Check if Queue Family support presentation
            VkBool32 presentation_support = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, idx, surface,
                                                 &presentation_support); // TODO: validar se result OK
            // check if queue is presentation type (can bo boyh graphics and presentation)
            if ((queue_family.queueCount > 0) && (presentation_support == VK_TRUE)) {
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

    SwapChainDetails VulkanContext::get_swap_chain_details(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SwapChainDetails swap_chain_details;

        // -- CAPABILITIES --
        // Get the surface capabilities for the given surface on the given physical device
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swap_chain_details.surfaceCapabilities);

        // -- FORMATS --
        uint32_t format_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

        // If formats returned, get list of formats
        if (format_count != 0) {
            swap_chain_details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, swap_chain_details.formats.data());
        }

        // -- PRESENTATION MODES --
        uint32_t presentation_count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentation_count, nullptr);

        // If presentation modes returned, get list of presentation modes
        if (presentation_count != 0) {
            swap_chain_details.presentationModes.resize(presentation_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentation_count,
                                                      swap_chain_details.presentationModes.data());
        }

        return swap_chain_details;
    }

    VkFormat VulkanContext::choose_supported_format(VkPhysicalDevice device, const std::vector<VkFormat>& formats,
                                                    VkImageTiling tilling, VkFormatFeatureFlags feature_flags) {

        // Loop through options and find compatible one
        for (VkFormat format : formats) {

            // Get properties for give format on this device
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(device, format, &properties);

            // Depending on tiling choice, nned to check for difference bit flag
            if (tilling == VK_IMAGE_TILING_LINEAR &&
                (properties.linearTilingFeatures & feature_flags) == feature_flags) {
                //
                return format;
            }
            if (tilling == VK_IMAGE_TILING_OPTIMAL &&
                (properties.optimalTilingFeatures & feature_flags) == feature_flags) {
                //
                return format;
            }
        }

        throw std::runtime_error("Failed to find a matching format!");
    }
#pragma endregion statics

#pragma region private_methodo

    void VulkanContext::create_instance() {

        if (validation_enabled_ && !VulkanContext::check_validation_layer_support()) {
            throw std::runtime_error("Required Validation Layers not supported!");
        }

        // Create a List to hold instance extencios
        std::vector<const char*> instance_extensions = std::vector<const char*>();

        // set up extentions will use
        uint32_t hw_extention_count = 0; // may require multiple extentions

        const char* const* hw_extentions; // Extentions passed as array of cstring,
        ;                                 // so need pointer (the array) to pointer(the string)
        hw_extentions = SDL_Vulkan_GetInstanceExtensions(&hw_extention_count);

        // Add glwf extentions to list of extentions
        for (size_t i = 0; i < hw_extention_count; i++) {
            instance_extensions.push_back(hw_extentions[i]);
        }

        // If validation enabled, add extension to report validation debug info
        if (validation_enabled_) {
            instance_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }

        // check Instance Extentions suppoted..
        if (!VulkanContext::check_instance_extension_support(&instance_extensions)) {
            throw std::runtime_error("vkInstance does no suport requerid extentions!");
        }

        // MoInformation about the aplication itself
        // st data here doesn't affect program and is for developer convinience
        const VkApplicationInfo app_info{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "Vulkan app Teste",         // Custom name of the aplication
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0), // Version app
            .pEngineName = "No engine",                     // Engine name
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),      // engine version
            .apiVersion = VK_API_VERSION_1_4                // the version of vulkan
        };

        // Set a validation layer tha instace will use
        uint32_t enabled_layer_count = 0;
        const char* const* pp_enabled_layer_names = nullptr;
        if ((!validation_layers.empty()) && validation_enabled_) {
            enabled_layer_count = static_cast<uint32_t>(validation_layers.size());
            pp_enabled_layer_names = validation_layers.data();
        }

        // Create information for a VkInstance
        const VkInstanceCreateInfo create_info{.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                               // .pNext = nullptr,
                                               // .flags = VK_WHATEVER | WHAT_EVER2,
                                               .pApplicationInfo = &app_info,
                                               .enabledLayerCount = enabled_layer_count,
                                               .ppEnabledLayerNames = pp_enabled_layer_names,
                                               .enabledExtensionCount =
                                                   static_cast<uint32_t>(instance_extensions.size()),
                                               .ppEnabledExtensionNames = instance_extensions.data()};

        // Create instance
        if (vkCreateInstance(&create_info, nullptr, &this->instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan Instance");
        }
    }

    void VulkanContext::create_debug_callback() {
        // Only create callback if validation enabled
        if (!validation_enabled_) {
            return;
        }

        const VkDebugReportCallbackCreateInfoEXT callback_create_info{
            .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                     VK_DEBUG_REPORT_WARNING_BIT_EXT, // Which validation reports should initiate callback
            .pfnCallback = DebugCallback              // Pointer to callback function itself
        };

        // Create debug callback with custom create function

        if (CreateDebugReportCallbackEXT(this->instance, &callback_create_info, nullptr, &this->callback_) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to create Debug Callback!");
        }
    }

    void VulkanContext::create_surface() {
        // Create Surface (creates a surface creste info struct, runs the create surface function, returns result)
        if (!SDL_Vulkan_CreateSurface(this->window, this->instance, nullptr, &this->surface)) {
            throw std::runtime_error("Failed to create a surface!");
        }
    }

    void VulkanContext::get_new_physical_device() {
        // Enumerate Physical devices the vkInstance can access
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(this->instance, &device_count, nullptr);

        // if no devices avaible, then none suport Vulkan!
        if (device_count == 0) {
            throw std::runtime_error("Cant find GPUs that support Vulkan Instance");
        }

        // get List of Physical devices
        std::vector<VkPhysicalDevice> device_list(device_count);
        vkEnumeratePhysicalDevices(this->instance, &device_count, device_list.data());

        // mainDevice.physicalDevice = deviceList[0];
        for (const auto& device : device_list) {
            if (VulkanContext::check_device_suitable(device, surface)) {
                physical = device;
                break;
            }
        }

        // Get properties of our new device
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(physical, &device_properties);
        // minUniformBufferOffset = deviceProperties.limits.minUniformBufferOffsetAlignment;

        // if (!VulkanContext::checkDescriptorIndexingSupport(physical)) {
        //     throw std::runtime_error("Descriptor Indexing Support not allowed");
        // }
    }

    void VulkanContext::create_logical_device() {

        // Get the queue family indices for the chosen Physical device
        this->queueFamilyIndices = VulkanContext::get_queue_families(physical, surface);

        // vector for queue creation information, and set for family indices
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set<int> squeue_family_indices = {this->queueFamilyIndices.graphicsFamily,
                                               this->queueFamilyIndices.presentationFamily};

        // Queues the logical device needs to create and info to do so
        for (const int queue_famiy_index : squeue_family_indices) {

            const float priority = 1.0F;
            const VkDeviceQueueCreateInfo queue_create_info{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex =
                    static_cast<uint32_t>(queue_famiy_index), // The index of the family to create a from
                .queueCount = 1,                              // Numbers of queues to create
                .pQueuePriorities =
                    &priority, // Vulkan needs to know how to handle multiple queues, so decide priority (1 is hight)
            };

            queue_create_infos.push_back(queue_create_info);
        }

        VkPhysicalDeviceDescriptorIndexingFeatures indexing_features = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
            .descriptorBindingSampledImageUpdateAfterBind = VK_TRUE};

        // 2. Query physical device support to ensure your GPU handles it
        VkPhysicalDeviceFeatures2 device_features2 = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
                                                      .pNext = &indexing_features};

        vkGetPhysicalDeviceFeatures2(this->physical, &device_features2);

        if (indexing_features.descriptorBindingSampledImageUpdateAfterBind == VK_FALSE) {
            throw std::runtime_error("GPU does not support updating sampled images after bind!");
        }

        // Information to create logical device (sometimes called "device")
        // Physical Device Features the Logical Device will be using
        const VkPhysicalDeviceFeatures device_features{
            .samplerAnisotropy = VK_TRUE // enable Anisotropy
        };

        const VkDeviceCreateInfo device_create_info{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = &indexing_features,                                              // bindlessTex
            .queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()), // Number queueCreateInfos
            .pQueueCreateInfos =
                queue_create_infos.data(), // List of queueCreateInfos so device can create required queues
            .enabledExtensionCount = static_cast<uint32_t>(
                VulkanContext::device_extensions.size()), // Number of enable logical device extentions
            .ppEnabledExtensionNames =
                VulkanContext::device_extensions.data(), // List of enable logical device extentions
            .pEnabledFeatures = &device_features         // Physica device features logica device will use
        };

        // Create the Logical device for the givem physical device
        if (vkCreateDevice(this->physical, &device_create_info, nullptr, &this->logical) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a logical device");
        }

        // queues are created the same time as the device
        vkGetDeviceQueue(this->logical, this->queueFamilyIndices.graphicsFamily, 0, &this->graphicsQueue);
        vkGetDeviceQueue(this->logical, this->queueFamilyIndices.presentationFamily, 0, &this->presentationQueue);
    }

    void VulkanContext::create_graphics_pool() {

        const VkCommandPoolCreateInfo pool_info{.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                                                .queueFamilyIndex =
                                                    static_cast<uint32_t>(queueFamilyIndices.graphicsFamily)};

        // Create a Graphics Queue Family Command Pool
        if (vkCreateCommandPool(this->logical, &pool_info, nullptr, &this->commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Command Pool");
        }
    }

    //--STATICS

    bool VulkanContext::check_device_extension_support(VkPhysicalDevice device) {
        // Get device extension count
        uint32_t extension_count = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

        // If no extensions found, return failure
        if (extension_count == 0) {
            return false;
        }

        // Populate list of extensions
        std::vector<VkExtensionProperties> extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extensions.data());

        // Check for extension
        for (const auto& device_extension : VulkanContext::device_extensions) {
            bool has_extension = false;
            for (const auto& extension : extensions) {
                if (std::strcmp(device_extension, extension.extensionName) == 0) {
                    has_extension = true;
                    break;
                }
            }

            if (!has_extension) {
                return false;
            }
        }

        return true;
    }

    bool VulkanContext::check_descriptor_indexing_support(VkPhysicalDevice device) {
        // 1. Instanciar a estrutura específica que queremos checar
        VkPhysicalDeviceDescriptorIndexingFeatures indexing_features = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
            .descriptorBindingSampledImageUpdateAfterBind = VK_TRUE};

        // 2. Instanciar a estrutura base de recursos modernos
        VkPhysicalDeviceFeatures2 device_features2 = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &indexing_features // Encadeia para preencher os dados de indexação
        };

        // 3. Consultar o driver da GPU
        vkGetPhysicalDeviceFeatures2(device, &device_features2);

        // 4. Validar os três recursos essenciais para texturas bindless
        bool has_dynamic_indexing = indexing_features.shaderSampledImageArrayNonUniformIndexing == VK_TRUE;
        bool has_partially_bound = indexing_features.descriptorBindingPartiallyBound == VK_TRUE;
        bool has_update_after_bind = indexing_features.descriptorBindingSampledImageUpdateAfterBind == VK_TRUE;

        return has_dynamic_indexing && has_partially_bound && has_update_after_bind;
    }

    bool VulkanContext::check_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface) {

        /*
        // Information abaout the device itself (ID, Name, Type Vendor, etc)
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        */
        // information about what the device can do (geo, Shader, tess, shader, wide lines, etc)
        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceFeatures(device, &device_features);

        QueueFamilyIndices indices = VulkanContext::get_queue_families(device, surface);

        bool extensions_supported = VulkanContext::check_device_extension_support(device);

        bool swap_chain_valid = false;
        if (extensions_supported) {
            SwapChainDetails swap_chain_details = VulkanContext::get_swap_chain_details(device, surface);
            swap_chain_valid = !swap_chain_details.presentationModes.empty() && !swap_chain_details.formats.empty();
        }

        return indices.isValid() && extensions_supported && swap_chain_valid &&
               (device_features.samplerAnisotropy == VK_TRUE);
    }

    bool VulkanContext::check_instance_extension_support(std::vector<const char*>* check_extentions) {
        // need to get number of extentions to create array of correct size to hold extentions
        uint32_t extentions_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extentions_count, nullptr);

        // Create list of vKExtentionsProperties using count
        std::vector<VkExtensionProperties> extentions(extentions_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &extentions_count, extentions.data());

        SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Extentions: ");
        // check if give extentions are list of avaible extentins
        for (const auto& check_extention : *check_extentions) {
            bool has_extentions = false;
            for (const auto& extention : extentions) {
                if (std::strcmp(check_extention, extention.extensionName) == 0) {

                    SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "- %s", check_extention);
                    has_extentions = true;
                    break;
                }
            }

            if (!has_extentions) {
                return false;
            }
        }

        return true;
    }

    bool VulkanContext::check_validation_layer_support() {
        // Get number of validation layers to create vector of appropriate size
        uint32_t validation_layer_count = 0;
        vkEnumerateInstanceLayerProperties(&validation_layer_count, nullptr);

        // Check if no validation layers found AND we want at least 1 layer
        if ((!validation_layers.empty()) && (validation_layer_count == 0)) {
            return false;
        }

        std::vector<VkLayerProperties> available_layers(validation_layer_count);
        vkEnumerateInstanceLayerProperties(&validation_layer_count, available_layers.data());

        SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Camadas Vulkan Disponiveis %d", validation_layer_count);

        for (const auto& prop : available_layers) {
            SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "----------------------------------");
            SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Layer Name: %s", prop.layerName);
            SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Description:  %s", prop.description);
            SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Implementation Version: %d", prop.implementationVersion);
            SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Spec Version: %d", prop.specVersion);
        }
        SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "----------------------------------");

        // Check if given Validation Layer is in list of given Validation Layers
        for (const auto& validation_layer : validation_layers) {
            bool has_layer = false;
            for (const auto& available_layer : available_layers) {
                if (std::strcmp(validation_layer, available_layer.layerName) == 0) {
                    has_layer = true;
                    break;
                }
            }

            if (!has_layer) {
                return false;
            }
        }

        return true;
    }

#pragma endregion private_methodo
} // namespace ce
