#include "SwapChain.hpp"
#include <SDL3/SDL_log.h>
#include <vulkan/vulkan_core.h>

namespace ce {

    void SwapChain::init(std::shared_ptr<VulkanContext> ctx, VkRenderPass renderPass, bool depthBufferEnable) {

        this->ctx = ctx;
        this->renderpass = renderPass;
        this->createSwapchain(depthBufferEnable, false);
    }

    void SwapChain::destroy() {

        if (this->ctx != nullptr) {
            this->depthBuffer.reset();
            this->swapchainData.destroy();
        }
    }

    void SwapChain::createSwapchain(bool depthBufferEnable, bool rebuild) {
        //
        SetupSwapchain setup = this->setupParams();
        // Get Swap Chain details so we cam pick best setting
        this->surfaceFormat = setup.surfaceFormat;
        this->extent = setup.extent;
        this->renderArea = {.offset = {.x = 0, .y = 0}, .extent = this->extent};

        if (depthBufferEnable) {
            if (this->depthBuffer) {
                this->depthBuffer.reset();
            }

            this->depthBuffer = std::make_shared<DepthBufferImage>(this->ctx, this->extent);
        }

        uint32_t queueFamilyIndexCount = 0;
        const uint32_t* pQueueFamilyIndices = nullptr;

        if (setup.imageSharingMode == VK_SHARING_MODE_CONCURRENT) {
            queueFamilyIndexCount = setup.queueFamilyIndices.size();
            pQueueFamilyIndices = setup.queueFamilyIndices.data();
        }

        // Guardamos o ponteiro da swapchain antiga (se houver) para otimizar a criação
        VkSwapchainKHR oldSwapchain = rebuild ? swapchainData.swapchain : VK_NULL_HANDLE;

        // Create information for swap chain
        const VkSwapchainCreateInfoKHR swapchainCreateInfo{
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = this->ctx->surface,                       // Swapchain surface
            .minImageCount = setup.imageCount,                   // Minimum image in swapchain
            .imageFormat = this->surfaceFormat.format,           // Swapchain format
            .imageColorSpace = this->surfaceFormat.colorSpace,   // Swapchain color space
            .imageExtent = this->extent,                         // Swapchain image extents
            .imageArrayLayers = 1,                               // Number of layers for each image in chain
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,   // What attachement image will be used as
            .imageSharingMode = setup.imageSharingMode,          // Image share handling
            .queueFamilyIndexCount = queueFamilyIndexCount,      // Number of queues to share images between
            .pQueueFamilyIndices = pQueueFamilyIndices,          // Array of queues to share between
            .preTransform = setup.currentTransform,              // Transform to perform on swap chain images
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, // How to handle blending images with external
                                                                 // graphics(e.g. other windows)
            .presentMode = setup.presentMode,                    // Swapchain presentation mode
            .clipped =
                VK_TRUE, // Whether to clip parts of image not in view (e.g. behind another window, off screen, etc)
            .oldSwapchain = oldSwapchain}; //  If old swap chain been destroyed and this one replaces it, then link
                                           //  old one to quickly hand over  responsabilities

        // Create Swapchain
        VkSwapchainKHR rawSwapchain;
        if (vkCreateSwapchainKHR(ctx->logical, &swapchainCreateInfo, nullptr, &rawSwapchain) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a Swapchain");
        }

        this->swapchainData = SwapchainData(ctx->logical, rawSwapchain);

        // Get swap chain images (first count the values)
        uint32_t swapChainImageCount;
        vkGetSwapchainImagesKHR(ctx->logical, this->swapchainData.swapchain, &swapChainImageCount, nullptr);
        std::vector<VkImage> swapchainImages(swapChainImageCount);
        vkGetSwapchainImagesKHR(ctx->logical, this->swapchainData.swapchain, &swapChainImageCount,
                                swapchainImages.data());

        this->swapchainData.images.resize(swapChainImageCount);

        for (size_t i = 0; i < swapChainImageCount; i++) {
            this->swapchainData.images[i] = {};
            this->swapchainData.images[i].create(ctx->logical, swapchainImages[i], this->renderpass, this->extent,
                                                 this->surfaceFormat.format, this->depthBuffer->getImageView());
        }
    }

    // Rotina de recriação total da Swapchain
    void SwapChain::recreateSwapchain() {
        // FIXME: nao esta funcionando corretamente
        // Trata o caso do aplicativo ser minimizado (largura ou altura igual a 0)
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx->physical, ctx->surface, &capabilities);
        uint32_t count = 0;

        while (capabilities.currentExtent.width == 0 || capabilities.currentExtent.height == 0) {

            SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Swapchain recreate (%d x %d)", capabilities.currentExtent.width,
                         capabilities.currentExtent.height);

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx->physical, ctx->surface, &capabilities);
            SDL_Delay(1000); // FIXME: signal ??
            SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "waiting (%d)..", count++);
        }

        // Aguarda a GPU terminar de renderizar qualquer frame pendente antes de destruir os alvos
        vkDeviceWaitIdle(this->ctx->logical);

        // Recria apenas os recursos dependentes do tamanho da tela
        createSwapchain((this->depthBuffer != nullptr), true);
    }

    std::pair<uint32_t, SwapchainImageResource&> SwapChain::acquireNextImage(VkFence& inFlightFence,
                                                                             VkSemaphore& waitImage) {

        // Sincronizar CPU com o Frame Virtual Atual
        vkWaitForFences(ctx->logical, 1, &inFlightFence, VK_TRUE, UINT64_MAX);

        // Get index of next image to be draw to, and signal semaphore when ready to be draw to
        uint32_t imageIndex;
        vkAcquireNextImageKHR(ctx->logical, this->swapchainData.swapchain, std::numeric_limits<uint64_t>::max(),
                              waitImage, VK_NULL_HANDLE, &imageIndex);

        // Se a imagem real adquirida ainda estiver sendo usada por algum frame virtual anterior, aguarde.
        this->swapchainData.images[imageIndex].syncImg(ctx->logical, inFlightFence);

        // Resetar a Fence do frame virtual para o estado não-sinalizado antes de enviar novos comandos
        vkResetFences(ctx->logical, 1, &inFlightFence);

        return {imageIndex, this->swapchainData.images[imageIndex]};
    }

    VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities) {

        // If current extend!!!!!!!!!!!!
        if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return surfaceCapabilities.currentExtent;
        }

        int witdh;
        int height;
        SDL_GetWindowSizeInPixels(ctx->window, &witdh, &height);

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

#pragma region auxiliar

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

#pragma endregion auxiliar

    SetupSwapchain SwapChain::setupParams() {

        SetupSwapchain setup;

        // Get Swap Chain details so we cam pick best setting
        SwapChainDetails swapchainDetails = VulkanContext::GetSwapChainDetails(ctx->physical, ctx->surface);

        setup.currentTransform = swapchainDetails.surfaceCapabilities.currentTransform;

        // Find optimal surface value for our swap chain,  Store for late reference
        setup.surfaceFormat = SwapChain::ChooseBestSurfaceFormat(swapchainDetails.formats);
        setup.presentMode = SwapChain::ChooseBestPresentationMode(swapchainDetails.presentationModes);
        setup.extent = this->chooseSwapExtent(swapchainDetails.surfaceCapabilities);

        // how many images are in the swap chain? Get 1 more than the minimum to allow triple buffering
        setup.imageCount = swapchainDetails.surfaceCapabilities.minImageCount + 1;

        // If imagecount higher than max the clamp down to max
        // If 0, then limitless
        if (swapchainDetails.surfaceCapabilities.maxImageCount > 0 &&
            swapchainDetails.surfaceCapabilities.maxImageCount < setup.imageCount) {
            setup.imageCount = swapchainDetails.surfaceCapabilities.maxImageCount;
        }

        // If Graphics and Presentation families are diferent, the swapchain must let images ge shared between families
        // indices.graphicsFamily == indices.presentationFamily
        setup.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (ctx->queueFamilyIndices.graphicsFamily != ctx->queueFamilyIndices.presentationFamily) {
            setup.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            setup.queueFamilyIndices.push_back(ctx->queueFamilyIndices.graphicsFamily);
            setup.queueFamilyIndices.push_back(ctx->queueFamilyIndices.presentationFamily);
        } else {
            setup.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            setup.queueFamilyIndices.push_back(ctx->queueFamilyIndices.graphicsFamily);
        }

        return setup;
    }

} // namespace ce
