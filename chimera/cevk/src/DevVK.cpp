#include "DevVK.hpp"
#include "debug.hpp"
#include <SDL3/SDL_vulkan.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>

namespace ce {

    DevVk::~DevVk() {
        // cleanup
        vkDestroySurfaceKHR(this->instance, this->bvk->surface, nullptr);
        vkDestroyDevice(this->bvk->logical, nullptr);

        if (this->validationEnabled) {
            DestroyDebugReportCallbackEXT(this->instance, this->callback, nullptr);
        }

        vkDestroyInstance(this->instance, nullptr);
    }

    void DevVk::init_device() {
        this->createInstance();
        this->createDebugCallback();
        this->createSurface();        // create before physical
        this->getNewPhysicalDevice(); // now need see if support surface
        this->createLogicalDevice();
    }

    void DevVk::createInstance() {

        if (this->validationEnabled && !DevVk::CheckValidationLayerSupport()) {
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
        if (!DevVk::CheckInstanceExtensionSupport(&instanceExtensions)) {
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
            .apiVersion = VK_API_VERSION_1_0                // the version of vulkan
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

    void DevVk::createDebugCallback() {
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

    void DevVk::createSurface() {
        // Create Surface (creates a surface creste info struct, runs the create surface function, returns result)
        if (!SDL_Vulkan_CreateSurface(this->bvk->window, this->instance, nullptr, &this->bvk->surface)) {
            throw std::runtime_error("Failed to create a surface!");
        }
    }

    void DevVk::getNewPhysicalDevice() {
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
            if (DevVk::CheckDeviceSuitable(device, this->bvk->surface)) {
                this->bvk->physical = device;
                break;
            }
        }

        // Get properties of our new device
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(this->bvk->physical, &deviceProperties);
        // minUniformBufferOffset = deviceProperties.limits.minUniformBufferOffsetAlignment;
    }

    void DevVk::createLogicalDevice() {

        // Get the queue family indices for the chosen Physical device
        QueueFamilyIndices indices = aux::GetQueueFamilies(this->bvk->physical, this->bvk->surface);

        // vector for queue creation information, and set for family indices
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<int> queueFamilyIndices = {indices.graphicsFamily, indices.presentationFamily};

        // Queues the logical device needs to create and info to do so
        for (const int queueFamiyIndex : queueFamilyIndices) {

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

        // Information to create logical device (sometimes called "device")
        // Physical Device Features the Logical Device will be using
        const VkPhysicalDeviceFeatures deviceFeatures{
            .samplerAnisotropy = VK_TRUE // enable Anisotropy
        };

        const VkDeviceCreateInfo deviceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()), // Number queueCreateInfos
            .pQueueCreateInfos =
                queueCreateInfos.data(), // List of queueCreateInfos so device can create required queues
            .enabledExtensionCount =
                static_cast<uint32_t>(DevVk::deviceExtensions.size()), // Number of enable logical device extentions
            .ppEnabledExtensionNames = DevVk::deviceExtensions.data(), // List of enable logical device extentions
            .pEnabledFeatures = &deviceFeatures                        // Physica device features logica device will use
        };

        // Create the Logical device for the givem physical device
        if (vkCreateDevice(this->bvk->physical, &deviceCreateInfo, nullptr, &this->bvk->logical) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a logical device");
        }

        // queues are created the same time as the device
        // so we want handle to queues
        // From given logical device, of given Queue Family, of given Queue Index(0 since only one), place reference in
        // given Vkqueue
        vkGetDeviceQueue(this->bvk->logical, indices.graphicsFamily, 0, &this->graphicsQueue);
        vkGetDeviceQueue(this->bvk->logical, indices.presentationFamily, 0, &this->presentationQueue);
    }

    // --utils
    bool DevVk::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
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
        for (const auto& deviceExtension : DevVk::deviceExtensions) {
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

    bool DevVk::CheckDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {

        /*
        // Information abaout the device itself (ID, Name, Type Vendor, etc)
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        */
        // information about what the device can do (geo, Shader, tess, shader, wide lines, etc)
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        QueueFamilyIndices indices = aux::GetQueueFamilies(device, surface);

        bool extensionsSupported = DevVk::CheckDeviceExtensionSupport(device);

        bool swapChainValid = false;
        if (extensionsSupported) {
            SwapChainDetails swapChainDetails = aux::GetSwapChainDetails(device, surface);
            swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();
        }

        return indices.isValid() && extensionsSupported && swapChainValid &&
               (deviceFeatures.samplerAnisotropy == VK_TRUE);
    }

    bool DevVk::CheckInstanceExtensionSupport(std::vector<const char*>* checkExtentions) {
        // need to get number of extentions to create array of correct size to hold extentions
        uint32_t extentionsCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extentionsCount, nullptr);

        // Create list of vKExtentionsProperties using count
        std::vector<VkExtensionProperties> extentions(extentionsCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extentionsCount, extentions.data());

        // check if give extentions are list of avaible extentins
        for (const auto& checkExtention : *checkExtentions) {
            bool hasExtentions = false;
            for (const auto& extention : extentions) {
                if (std::strcmp(checkExtention, extention.extensionName) == 0) {
                    std::cout << "Extenções: " << checkExtention << '\n';
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

    bool DevVk::CheckValidationLayerSupport() {
        // Get number of validation layers to create vector of appropriate size
        uint32_t validationLayerCount = 0;
        vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);

        // Check if no validation layers found AND we want at least 1 layer
        if (validationLayerCount == 0 && validationLayers.size() > 0) {
            return false;
        }

        std::vector<VkLayerProperties> availableLayers(validationLayerCount);
        vkEnumerateInstanceLayerProperties(&validationLayerCount, availableLayers.data());

        std::cout << "Camadas Vulkan Disponiveis (" << validationLayerCount << "):" << '\n';
        for (const auto& layerProperties : availableLayers) {
            std::cout << "\tLayer Name: " << layerProperties.layerName << '\n';
            std::cout << "\tDescription: " << layerProperties.description << '\n';
            std::cout << "\tImplementation Version: " << layerProperties.implementationVersion << '\n';
            std::cout << "\tSpec Version: " << layerProperties.specVersion << '\n';
            std::cout << "\t-----------------------------------" << '\n';
        }

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

    namespace aux {

        SwapChainDetails GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface) {
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

        VkFormat ChooseSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& formats,
                                       VkImageTiling tilling, VkFormatFeatureFlags featureFlags) {

            // Loop through options and find compatible one
            for (VkFormat format : formats) {

                // Get properties for give format on this device
                VkFormatProperties properties;
                vkGetPhysicalDeviceFormatProperties(device, format, &properties);

                // Depending on tiling choice, nned to check for difference bit flag
                if (tilling == VK_IMAGE_TILING_LINEAR &&
                    (properties.linearTilingFeatures & featureFlags) == featureFlags) {
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

        QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {

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

        // --swapchain

        uint32_t FindMemoryTypeIndex(VkPhysicalDevice physicalDevice, uint32_t allowedTypes,
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

        // --utils

        std::vector<char> readFile(const std::filesystem::path& filename) {
            // Open stream from given file
            // std::ios::binary tells stream to read file as binary
            // std::ios::ate tells stream to start reading from end file
            std::ifstream file(filename, std::ios::binary | std::ios::ate);

            // Chack if fstream sucessfully open
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open a file!");
            }

            auto filesize = static_cast<size_t>(file.tellg());

            std::vector<char> fileBuffer(filesize);

            // Move read position (seek to0 the start of the file)
            file.seekg(0);

            // Read the file data into the buffer (stream "fileSize" in total)
            file.read(fileBuffer.data(), filesize);

            // Close stream
            file.close();

            return fileBuffer;
        }
    } // namespace aux

} // namespace ce
