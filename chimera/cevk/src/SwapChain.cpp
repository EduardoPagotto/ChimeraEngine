#include "SwapChain.hpp"
#include "DevVK.hpp"
#include <array>

namespace ce {

    SwapChain::SwapChain(BaseVK* pBVK) : physical(pBVK->physical), logical(pBVK->logical), window(pBVK->window) {

        // Get Swap Chain details so we cam pick best setting
        SwapChainDetails swapchainDetails = aux::GetSwapChainDetails(pBVK->physical, pBVK->surface);

        // Find optimal surface value for our swap chain
        VkSurfaceFormatKHR surrfaceFormat = SwapChain::ChooseBestSurfaceFormat(swapchainDetails.formats);

        VkPresentModeKHR presentMode = SwapChain::ChooseBestPresentationMode(swapchainDetails.presentationModes);
        VkExtent2D extent = this->chooseSwapExtent(swapchainDetails.surfaceCapabilities);

        // how many images are in the swap chain? Get 1 more than the minimum to allow triple buffering
        uint32_t imageCount = swapchainDetails.surfaceCapabilities.minImageCount + 1;

        // If imagecount higher than max the clamp down to max
        // If 0, then limitless
        if (swapchainDetails.surfaceCapabilities.maxImageCount > 0 &&
            swapchainDetails.surfaceCapabilities.maxImageCount < imageCount) {
            imageCount = swapchainDetails.surfaceCapabilities.maxImageCount;
        }

        // Get Queue Family indices
        QueueFamilyIndices indices = aux::GetQueueFamilies(pBVK->physical, pBVK->surface);
        // If Graphics and Presentation families are diferent, the swapchain must let images ge shared between families

        // indices.graphicsFamily == indices.presentationFamily
        VkSharingMode imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        uint32_t queueFamilyIndexCount = 0;
        const uint32_t* pQueueFamilyIndices = nullptr;

        // If Graphics and Presentation families are diferent, the swapchain must let images ge shared between families
        if (indices.graphicsFamily != indices.presentationFamily) {
            // Queue to share between
            std::array<uint32_t, 2> queueFamilyIndices = {static_cast<uint32_t>(indices.graphicsFamily),
                                                          static_cast<uint32_t>(indices.presentationFamily)};

            imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
            pQueueFamilyIndices = queueFamilyIndices.data();
        }

        // Create information for swap chain
        const VkSwapchainCreateInfoKHR swapchainCreateInfo{
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = pBVK->surface,                          // Swapchain surface
            .minImageCount = imageCount,                       // Minimum image in swapchain
            .imageFormat = surrfaceFormat.format,              // Swapchain format
            .imageColorSpace = surrfaceFormat.colorSpace,      // Swapchain color space
            .imageExtent = extent,                             // Swapchain image extents
            .imageArrayLayers = 1,                             // Number of layers for each image in chain
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, // What attachement image will be used as
            .imageSharingMode = imageSharingMode,              // Image share handling
            .queueFamilyIndexCount = queueFamilyIndexCount,    // Number of queues to share images between
            .pQueueFamilyIndices = pQueueFamilyIndices,        // Array of queues to share between
            .preTransform =
                swapchainDetails.surfaceCapabilities.currentTransform, // Transform to perform on swap chain images
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,       // How to handle blending images with external
                                                                       // graphics(e.g. other windows)
            .presentMode = presentMode,                                // Swapchain presentation mode
            .clipped =
                VK_TRUE, // Whether to clip parts of image not in view (e.g. behind another window, off screen, etc)
            .oldSwapchain = VK_NULL_HANDLE}; //  If old swap chain been destroyed and this one replaces it, then link
                                             //  old one to quickly hand over  responsabilities

        // Create Swapchain
        if (vkCreateSwapchainKHR(pBVK->logical, &swapchainCreateInfo, nullptr, &this->swapchain) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a Swapchain");
        }

        // Store for late reference
        this->imageFormat = surrfaceFormat.format;
        this->extent = extent;

        // Get swap chain images (first count the values)
        uint32_t swapChainImageCount;
        vkGetSwapchainImagesKHR(pBVK->logical, this->swapchain, &swapChainImageCount, nullptr);

        std::vector<VkImage> lImages(swapChainImageCount);
        vkGetSwapchainImagesKHR(pBVK->logical, this->swapchain, &swapChainImageCount, lImages.data());

        for (VkImage image : lImages) {

            auto imgObj = std::make_shared<ImageObject>(pBVK->physical, pBVK->logical);
            imgObj->createImageViewImportedImage(image, this->imageFormat,
                                                 VK_IMAGE_ASPECT_COLOR_BIT); // CreateImageView
            this->images.push_back(imgObj);
        }
    }

    SwapChain::~SwapChain() {

        for (auto& framebuffer : this->swapChainFrameBuffers) { // ? auto& mesmo ??
            vkDestroyFramebuffer(this->logical, framebuffer, nullptr);
        }

        for (auto& image : this->images) {
            image.reset();
        }

        // TODO: e aqui?
        this->depthBufferObject.reset();

        vkDestroySwapchainKHR(this->logical, this->swapchain, nullptr);
    }

    VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities) {

        // If current extend!!!!!!!!!!!!
        if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return surfaceCapabilities.currentExtent;
        }

        int witdh;
        int height;
        SDL_GetWindowSizeInPixels(this->window, &witdh, &height);

        VkExtent2D newExtent{
            .width = static_cast<uint32_t>(witdh),  //
            .height = static_cast<uint32_t>(height) //
        };

        // surface also defie max and min, so make sure within bondaries by clamping value
        newExtent.width = std::max(surfaceCapabilities.minImageExtent.width,
                                   std::min(surfaceCapabilities.maxImageExtent.width, newExtent.width));

        newExtent.height = std::max(surfaceCapabilities.minImageExtent.height,
                                    std::min(surfaceCapabilities.maxImageExtent.height, newExtent.height));

        return newExtent;
    }

    void SwapChain::createFramebuffers(VkRenderPass& renderPass) {

        // create depth buffer
        this->createDepthBufferImage();

        // Resize framebuffer count to equal chain image count
        this->swapChainFrameBuffers.resize(this->images.size());

        // Create a framebuffer for eache swap chain image
        for (size_t i = 0; i < this->swapChainFrameBuffers.size(); i++) {

            std::array<VkImageView, 2> attachments = {
                this->images[i]->getImageView(), depthBufferObject->getImageView()}; // order important same as upper

            const VkFramebufferCreateInfo framebufferCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .renderPass = renderPass, // Render Pass layout the framebuffer will be used with
                .attachmentCount = static_cast<uint32_t>(attachments.size()), //
                .pAttachments = attachments.data(), // List of attachments (1:1 with Render Pass)
                .width = this->extent.width,        // Framebuffer width
                .height = this->extent.height,      // Framebuffer height
                .layers = 1                         // Framebuffer layers
            };

            if (vkCreateFramebuffer(this->logical, &framebufferCreateInfo, nullptr, &this->swapChainFrameBuffers[i]) !=
                VK_SUCCESS) {
                throw std::runtime_error("Faleid to create a frambuffer");
            }
        }
    }

    void SwapChain::createDepthBufferImage() {

        // Get suported format for depth buffer
        VkFormat depthFormat = aux::ChooseSupportedFormat(
            this->physical,
            {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT}, // Formats
            VK_IMAGE_TILING_OPTIMAL,                                                           // Tilling
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);                                   // Depth

        // Create Depth Buffer Image
        this->depthBufferObject = std::make_shared<ImageObject>(this->physical, this->logical);
        this->depthBufferObject->createImage(this->extent.width, this->extent.height, depthFormat,
                                             VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Create Depth Buffer Image View
        this->depthBufferObject->createImageView(VK_IMAGE_ASPECT_DEPTH_BIT);
    }

    void SwapChain::sendImageToScreen(VkQueue pQueue, VkSemaphore signal, uint32_t& imageIndex) {
        //
        // -- PRESENT RENDERED IMAGE TO SCREEN --
        std::array<VkSemaphore, 1> signalSemaphores{signal}; //{this->sync->getSignalSemaphore(this->currentFrame)};
        std::array<VkSwapchainKHR, 1> swapChains{this->swapchain};

        const VkPresentInfoKHR presentInfo{
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()), // Number of semaphores to wait on
            .pWaitSemaphores = signalSemaphores.data(),                           // Semaphores to wait on
            .swapchainCount = static_cast<uint32_t>(swapChains.size()),           // Number of swapchains to present to
            .pSwapchains = swapChains.data(),                                     // Swapchais to present images to
            .pImageIndices = &imageIndex, // Index of Images in swapchains to present
        };

        // Present Image
        if (vkQueuePresentKHR(pQueue, &presentInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to present Image!");
        }
    }

    // Best format is subjective, but ours will be:
    // Format     : VK_FORMAT_R8G8B8A8_UNFORM (VK_FORMAT_B8G8R8A8_UNORM as backup)
    // colorSpace : VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
    VkSurfaceFormatKHR SwapChain::ChooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) {

        // If only 1 format avaible and is undefined, them this means ALL formats ase avaible (no restricion)
        if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
            return {.format = VK_FORMAT_R8G8B8A8_UNORM, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
        }

        // If restriced, searche for optimal format
        for (const auto& format : formats) {
            if ((format.format == VK_FORMAT_R8G8B8A8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM) &&
                format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
        }

        // If can't find optimal format, then just return first format
        return formats[0]; // FIXME: pade data pau aqui
    }

    VkPresentModeKHR SwapChain::ChooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes) {
        // Look for Mailbox presentation mode
        for (const auto& presentationMode : presentationModes) {
            if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return presentationMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR; // allways avaible by vulkan
    }

} // namespace ce
