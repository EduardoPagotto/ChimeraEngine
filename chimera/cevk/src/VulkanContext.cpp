#include "VulkanContext.hpp"
#include "debug.hpp"
#include <SDL3/SDL_vulkan.h>
#include <cstring>
#include <format>
#include <set>
#include <stdexcept>

namespace ce {

    void VulkanContext::createWindow(const std::string& sName, const int& width, const int& height) {

        // 1. Initialize SDL3
        if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland")) {
            throw std::runtime_error(std::format("SDL wayland Failed driver: {}", SDL_GetError()));
        }

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error(std::format("SDL Video Failed: {}", SDL_GetError()));
        }

        // 2. Create Window with Vulkan support
        this->window = SDL_CreateWindow(sName.c_str(), width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
        if (this->window == nullptr) {
            throw std::runtime_error(std::format("SDL Window creation failed: {}", SDL_GetError()));
        }

        SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Vulkan SDL3 Window Created OK");

        this->init();
    }

    void VulkanContext::init() {

        this->createInstance();
        this->createDebugCallback();
        this->createSurface();
        this->getNewPhysicalDevice();
        this->createLogicalDevice();
        this->createGraphicsPool();
    }

    void VulkanContext::destroy() {
        if (this->commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(this->logical, this->commandPool, nullptr);
            this->commandPool = VK_NULL_HANDLE;
        }

        vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
        vkDestroyDevice(this->logical, nullptr);

        if (this->validationEnabled) {
            DestroyDebugReportCallbackEXT(this->instance, this->callback, nullptr);
        }

        vkDestroyInstance(this->instance, nullptr);

        SDL_DestroyWindow(this->window);
    }

#pragma region statics
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
#pragma endregion statics

#pragma region private_methodo

    void VulkanContext::createInstance() {

        if (this->validationEnabled && !VulkanContext::CheckValidationLayerSupport()) {
            throw std::runtime_error("Required Validation Layers not supported!");
        }

        // Create a List to hold instance extencios
        std::vector<const char*> instanceExtensions = std::vector<const char*>();

        // set up extentions will use
        uint32_t hwExtentionCount = 0; // may require multiple extentions

        const char* const* hwExtentions; // Extentions passed as array of cstring,
        ;                                // so need pointer (the array) to pointer(the string)
        hwExtentions = SDL_Vulkan_GetInstanceExtensions(&hwExtentionCount);

        // Add glwf extentions to list of extentions
        for (size_t i = 0; i < hwExtentionCount; i++) {
            instanceExtensions.push_back(hwExtentions[i]);
        }

        // If validation enabled, add extension to report validation debug info
        if (this->validationEnabled) {
            instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }

        // check Instance Extentions suppoted..
        if (!VulkanContext::CheckInstanceExtensionSupport(&instanceExtensions)) {
            throw std::runtime_error("vkInstance does no suport requerid extentions!");
        }

        // MoInformation about the aplication itself
        // st data here doesn't affect program and is for developer convinience
        const VkApplicationInfo appInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "Vulkan app Teste",         // Custom name of the aplication
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0), // Version app
            .pEngineName = "No engine",                     // Engine name
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),      // engine version
            .apiVersion = VK_API_VERSION_1_4                // the version of vulkan
        };

        // Set a validation layer tha instace will use
        uint32_t enabledLayerCount = 0;
        const char* const* ppEnabledLayerNames = nullptr;
        if (this->validationEnabled && validationLayers.size() > 0) {
            enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            ppEnabledLayerNames = validationLayers.data();
        }

        // Create information for a VkInstance
        const VkInstanceCreateInfo createInfo{.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                              // .pNext = nullptr,
                                              // .flags = VK_WHATEVER | WHAT_EVER2,
                                              .pApplicationInfo = &appInfo,
                                              .enabledLayerCount = enabledLayerCount,
                                              .ppEnabledLayerNames = ppEnabledLayerNames,
                                              .enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size()),
                                              .ppEnabledExtensionNames = instanceExtensions.data()};

        // Create instance
        if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan Instance");
        }
    }

    void VulkanContext::createDebugCallback() {
        // Only create callback if validation enabled
        if (!this->validationEnabled) {
            return;
        }

        const VkDebugReportCallbackCreateInfoEXT callbackCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                     VK_DEBUG_REPORT_WARNING_BIT_EXT, // Which validation reports should initiate callback
            .pfnCallback = DebugCallback              // Pointer to callback function itself
        };

        // Create debug callback with custom create function

        if (CreateDebugReportCallbackEXT(this->instance, &callbackCreateInfo, nullptr, &this->callback) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Debug Callback!");
        }
    }

    void VulkanContext::createSurface() {
        // Create Surface (creates a surface creste info struct, runs the create surface function, returns result)
        if (!SDL_Vulkan_CreateSurface(this->window, this->instance, nullptr, &this->surface)) {
            throw std::runtime_error("Failed to create a surface!");
        }
    }

    void VulkanContext::getNewPhysicalDevice() {
        // Enumerate Physical devices the vkInstance can access
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);

        // if no devices avaible, then none suport Vulkan!
        if (deviceCount == 0) {
            throw std::runtime_error("Cant find GPUs that support Vulkan Instance");
        }

        // get List of Physical devices
        std::vector<VkPhysicalDevice> deviceList(deviceCount);
        vkEnumeratePhysicalDevices(this->instance, &deviceCount, deviceList.data());

        // mainDevice.physicalDevice = deviceList[0];
        for (const auto& device : deviceList) {
            if (VulkanContext::CheckDeviceSuitable(device, surface)) {
                physical = device;
                break;
            }
        }

        // Get properties of our new device
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physical, &deviceProperties);
        // minUniformBufferOffset = deviceProperties.limits.minUniformBufferOffsetAlignment;

        // if (!VulkanContext::checkDescriptorIndexingSupport(physical)) {
        //     throw std::runtime_error("Descriptor Indexing Support not allowed");
        // }
    }

    void VulkanContext::createLogicalDevice() {

        // Get the queue family indices for the chosen Physical device
        this->queueFamilyIndices = VulkanContext::GetQueueFamilies(physical, surface);

        // vector for queue creation information, and set for family indices
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<int> squeueFamilyIndices = {this->queueFamilyIndices.graphicsFamily,
                                             this->queueFamilyIndices.presentationFamily};

        // Queues the logical device needs to create and info to do so
        for (const int queueFamiyIndex : squeueFamilyIndices) {

            const float priority = 1.0F;
            const VkDeviceQueueCreateInfo queueCreateInfo{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = static_cast<uint32_t>(queueFamiyIndex), // The index of the family to create a from
                .queueCount = 1,                                            // Numbers of queues to create
                .pQueuePriorities =
                    &priority, // Vulkan needs to know how to handle multiple queues, so decide priority (1 is hight)
            };

            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
            .descriptorBindingSampledImageUpdateAfterBind = VK_TRUE};

        // 2. Query physical device support to ensure your GPU handles it
        VkPhysicalDeviceFeatures2 deviceFeatures2 = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
                                                     .pNext = &indexingFeatures};

        vkGetPhysicalDeviceFeatures2(this->physical, &deviceFeatures2);

        if (!indexingFeatures.descriptorBindingSampledImageUpdateAfterBind) {
            throw std::runtime_error("GPU does not support updating sampled images after bind!");
        }

        // Information to create logical device (sometimes called "device")
        // Physical Device Features the Logical Device will be using
        const VkPhysicalDeviceFeatures deviceFeatures{
            .samplerAnisotropy = VK_TRUE // enable Anisotropy
        };

        const VkDeviceCreateInfo deviceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = &indexingFeatures,                                             // bindlessTex
            .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()), // Number queueCreateInfos
            .pQueueCreateInfos =
                queueCreateInfos.data(), // List of queueCreateInfos so device can create required queues
            .enabledExtensionCount = static_cast<uint32_t>(
                VulkanContext::deviceExtensions.size()), // Number of enable logical device extentions
            .ppEnabledExtensionNames =
                VulkanContext::deviceExtensions.data(), // List of enable logical device extentions
            .pEnabledFeatures = &deviceFeatures         // Physica device features logica device will use
        };

        // Create the Logical device for the givem physical device
        if (vkCreateDevice(this->physical, &deviceCreateInfo, nullptr, &this->logical) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a logical device");
        }

        // queues are created the same time as the device
        vkGetDeviceQueue(this->logical, this->queueFamilyIndices.graphicsFamily, 0, &this->graphicsQueue);
        vkGetDeviceQueue(this->logical, this->queueFamilyIndices.presentationFamily, 0, &this->presentationQueue);
    }

    void VulkanContext::createGraphicsPool() {

        const VkCommandPoolCreateInfo poolInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                                               .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                                               .queueFamilyIndex =
                                                   static_cast<uint32_t>(queueFamilyIndices.graphicsFamily)};

        // Create a Graphics Queue Family Command Pool
        if (vkCreateCommandPool(this->logical, &poolInfo, nullptr, &this->commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Command Pool");
        }
    }

    //--STATICS

    bool VulkanContext::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
        // Get device extension count
        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        // If no extensions found, return failure
        if (extensionCount == 0) {
            return false;
        }

        // Populate list of extensions
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());

        // Check for extension
        for (const auto& deviceExtension : VulkanContext::deviceExtensions) {
            bool hasExtension = false;
            for (const auto& extension : extensions) {
                if (std::strcmp(deviceExtension, extension.extensionName) == 0) {
                    hasExtension = true;
                    break;
                }
            }

            if (!hasExtension) {
                return false;
            }
        }

        return true;
    }

    bool VulkanContext::checkDescriptorIndexingSupport(VkPhysicalDevice device) {
        // 1. Instanciar a estrutura específica que queremos checar
        VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES,
            .descriptorBindingSampledImageUpdateAfterBind = VK_TRUE};

        // 2. Instanciar a estrutura base de recursos modernos
        VkPhysicalDeviceFeatures2 deviceFeatures2 = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &indexingFeatures // Encadeia para preencher os dados de indexação
        };

        // 3. Consultar o driver da GPU
        vkGetPhysicalDeviceFeatures2(device, &deviceFeatures2);

        // 4. Validar os três recursos essenciais para texturas bindless
        bool hasDynamicIndexing = indexingFeatures.shaderSampledImageArrayNonUniformIndexing == VK_TRUE;
        bool hasPartiallyBound = indexingFeatures.descriptorBindingPartiallyBound == VK_TRUE;
        bool hasUpdateAfterBind = indexingFeatures.descriptorBindingSampledImageUpdateAfterBind == VK_TRUE;

        return hasDynamicIndexing && hasPartiallyBound && hasUpdateAfterBind;
    }

    bool VulkanContext::CheckDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {

        /*
        // Information abaout the device itself (ID, Name, Type Vendor, etc)
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        */
        // information about what the device can do (geo, Shader, tess, shader, wide lines, etc)
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        QueueFamilyIndices indices = VulkanContext::GetQueueFamilies(device, surface);

        bool extensionsSupported = VulkanContext::CheckDeviceExtensionSupport(device);

        bool swapChainValid = false;
        if (extensionsSupported) {
            SwapChainDetails swapChainDetails = VulkanContext::GetSwapChainDetails(device, surface);
            swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();
        }

        return indices.isValid() && extensionsSupported && swapChainValid &&
               (deviceFeatures.samplerAnisotropy == VK_TRUE);
    }

    bool VulkanContext::CheckInstanceExtensionSupport(std::vector<const char*>* checkExtentions) {
        // need to get number of extentions to create array of correct size to hold extentions
        uint32_t extentionsCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extentionsCount, nullptr);

        // Create list of vKExtentionsProperties using count
        std::vector<VkExtensionProperties> extentions(extentionsCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extentionsCount, extentions.data());

        SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Extentions: ");
        // check if give extentions are list of avaible extentins
        for (const auto& checkExtention : *checkExtentions) {
            bool hasExtentions = false;
            for (const auto& extention : extentions) {
                if (std::strcmp(checkExtention, extention.extensionName) == 0) {

                    SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "- %s", checkExtention);
                    hasExtentions = true;
                    break;
                }
            }

            if (!hasExtentions) {
                return false;
            }
        }

        return true;
    }

    bool VulkanContext::CheckValidationLayerSupport() {
        // Get number of validation layers to create vector of appropriate size
        uint32_t validationLayerCount = 0;
        vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);

        // Check if no validation layers found AND we want at least 1 layer
        if (validationLayerCount == 0 && validationLayers.size() > 0) {
            return false;
        }

        std::vector<VkLayerProperties> availableLayers(validationLayerCount);
        vkEnumerateInstanceLayerProperties(&validationLayerCount, availableLayers.data());

        SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Camadas Vulkan Disponiveis %d", validationLayerCount);

        for (const auto& prop : availableLayers) {
            SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "----------------------------------");
            SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Layer Name: %s", prop.layerName);
            SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Description:  %s", prop.description);
            SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Implementation Version: %d", prop.implementationVersion);
            SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "Spec Version: %d", prop.specVersion);
        }
        SDL_LogInfo(SDL_LOG_CATEGORY_VIDEO, "----------------------------------");

        // Check if given Validation Layer is in list of given Validation Layers
        for (const auto& validationLayer : validationLayers) {
            bool hasLayer = false;
            for (const auto& availableLayer : availableLayers) {
                if (std::strcmp(validationLayer, availableLayer.layerName) == 0) {
                    hasLayer = true;
                    break;
                }
            }

            if (!hasLayer) {
                return false;
            }
        }

        return true;
    }

#pragma endregion private_methodo
} // namespace ce
