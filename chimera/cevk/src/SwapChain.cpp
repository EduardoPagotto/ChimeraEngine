#include "SwapChain.hpp"
#include <SDL3/SDL_log.h>
#include <vulkan/vulkan_core.h>

namespace ce {

    void SwapChain::init(std::shared_ptr<VulkanContext> ctx, VkRenderPass render_pass, bool depth_buffer_enable) {

        ctx_ = ctx;
        renderpass_ = render_pass;
        create_swapchain(depth_buffer_enable, false);
    }

    void SwapChain::destroy() {

        if (ctx_ != nullptr) {
            depth_buffer_.reset();
            swapchain_data_.destroy();
        }
    }

    void SwapChain::create_swapchain(bool depth_buffer_enable, bool rebuild) {
        //
        SetupSwapchain setup = setup_params();
        // Get Swap Chain details so we cam pick best setting
        surface_format_ = setup.surfaceFormat;
        extent_ = setup.extent;
        render_area_ = {.offset = {.x = 0, .y = 0}, .extent = extent_};

        if (depth_buffer_enable) {
            if (depth_buffer_) {
                depth_buffer_.reset();
            }

            depth_buffer_ = std::make_shared<DepthBufferImage>(ctx_, extent_);
        }

        uint32_t queue_family_index_count = 0;
        const uint32_t* p_queue_family_indices = nullptr;

        if (setup.imageSharingMode == VK_SHARING_MODE_CONCURRENT) {
            queue_family_index_count = setup.queueFamilyIndices.size();
            p_queue_family_indices = setup.queueFamilyIndices.data();
        }

        // Guardamos o ponteiro da swapchain antiga (se houver) para otimizar a criação
        VkSwapchainKHR old_swapchain = rebuild ? swapchain_data_.swapchain : VK_NULL_HANDLE;

        // Create information for swap chain
        const VkSwapchainCreateInfoKHR swapchain_create_info{
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = ctx_->surface,                            // Swapchain surface
            .minImageCount = setup.imageCount,                   // Minimum image in swapchain
            .imageFormat = surface_format_.format,               // Swapchain format
            .imageColorSpace = surface_format_.colorSpace,       // Swapchain color space
            .imageExtent = extent_,                              // Swapchain image extents
            .imageArrayLayers = 1,                               // Number of layers for each image in chain
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,   // What attachement image will be used as
            .imageSharingMode = setup.imageSharingMode,          // Image share handling
            .queueFamilyIndexCount = queue_family_index_count,   // Number of queues to share images between
            .pQueueFamilyIndices = p_queue_family_indices,       // Array of queues to share between
            .preTransform = setup.currentTransform,              // Transform to perform on swap chain images
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, // How to handle blending images with external
                                                                 // graphics(e.g. other windows)
            .presentMode = setup.presentMode,                    // Swapchain presentation mode
            .clipped =
                VK_TRUE, // Whether to clip parts of image not in view (e.g. behind another window, off screen, etc)
            .oldSwapchain = old_swapchain}; //  If old swap chain been destroyed and this one replaces it, then link
                                            //  old one to quickly hand over  responsabilities

        // Create Swapchain
        VkSwapchainKHR raw_swapchain;
        if (vkCreateSwapchainKHR(ctx_->logical, &swapchain_create_info, nullptr, &raw_swapchain) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a Swapchain");
        }

        swapchain_data_ = SwapchainData(ctx_->logical, raw_swapchain);

        // Get swap chain images (first count the values)
        uint32_t swap_chain_image_count;
        vkGetSwapchainImagesKHR(ctx_->logical, swapchain_data_.swapchain, &swap_chain_image_count, nullptr);
        std::vector<VkImage> swapchain_images(swap_chain_image_count);
        vkGetSwapchainImagesKHR(ctx_->logical, swapchain_data_.swapchain, &swap_chain_image_count,
                                swapchain_images.data());

        swapchain_data_.images.resize(swap_chain_image_count);

        for (size_t i = 0; i < swap_chain_image_count; i++) {
            swapchain_data_.images[i] = {};
            swapchain_data_.images[i].create(ctx_->logical, swapchain_images[i], renderpass_, extent_,
                                             surface_format_.format, depth_buffer_->get_image_view());
        }
    }

    // Rotina de recriação total da Swapchain
    void SwapChain::recreate_swapchain() {
        // FIXME: nao esta funcionando corretamente
        // Trata o caso do aplicativo ser minimizado (largura ou altura igual a 0)
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx_->physical, ctx_->surface, &capabilities);
        uint32_t count = 0;

        while (capabilities.currentExtent.width == 0 || capabilities.currentExtent.height == 0) {

            SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Swapchain recreate (%d x %d)", capabilities.currentExtent.width,
                         capabilities.currentExtent.height);

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx_->physical, ctx_->surface, &capabilities);
            SDL_Delay(1000); // FIXME: signal ??
            SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "waiting (%d)..", count++);
        }

        // Aguarda a GPU terminar de renderizar qualquer frame pendente antes de destruir os alvos
        vkDeviceWaitIdle(ctx_->logical);

        // Recria apenas os recursos dependentes do tamanho da tela
        create_swapchain((depth_buffer_ != nullptr), true);
    }

    std::pair<uint32_t, SwapchainImageResource&> SwapChain::acquire_next_image(VkFence& in_flight_fence,
                                                                               VkSemaphore& wait_image) {

        // Sincronizar CPU com o Frame Virtual Atual
        vkWaitForFences(ctx_->logical, 1, &in_flight_fence, VK_TRUE, UINT64_MAX);

        // Get index of next image to be draw to, and signal semaphore when ready to be draw to
        uint32_t image_index;
        vkAcquireNextImageKHR(ctx_->logical, swapchain_data_.swapchain, std::numeric_limits<uint64_t>::max(),
                              wait_image, VK_NULL_HANDLE, &image_index);

        // Se a imagem real adquirida ainda estiver sendo usada por algum frame virtual anterior, aguarde.
        swapchain_data_.images[image_index].sync_img(ctx_->logical, in_flight_fence);

        // Resetar a Fence do frame virtual para o estado não-sinalizado antes de enviar novos comandos
        vkResetFences(ctx_->logical, 1, &in_flight_fence);

        return {image_index, swapchain_data_.images[image_index]};
    }

    VkExtent2D SwapChain::choose_swap_extent(const VkSurfaceCapabilitiesKHR& surface_capabilities) {

        // If current extend!!!!!!!!!!!!
        if (surface_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return surface_capabilities.currentExtent;
        }

        int witdh;
        int height;
        SDL_GetWindowSizeInPixels(ctx_->window, &witdh, &height);

        VkExtent2D new_extent{
            .width = static_cast<uint32_t>(witdh),  //
            .height = static_cast<uint32_t>(height) //
        };

        // surface also defie max and min, so make sure within bondaries by clamping value
        new_extent.width = std::max(surface_capabilities.minImageExtent.width,
                                    std::min(surface_capabilities.maxImageExtent.width, new_extent.width));

        new_extent.height = std::max(surface_capabilities.minImageExtent.height,
                                     std::min(surface_capabilities.maxImageExtent.height, new_extent.height));

        return new_extent;
    }

#pragma region auxiliar

    // Best format is subjective, but ours will be:
    // Format     : VK_FORMAT_R8G8B8A8_UNFORM (VK_FORMAT_B8G8R8A8_UNORM as backup)
    // colorSpace : VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
    VkSurfaceFormatKHR SwapChain::choose_best_surface_format(const std::vector<VkSurfaceFormatKHR>& formats) {

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

    VkPresentModeKHR SwapChain::choose_best_presentation_mode(const std::vector<VkPresentModeKHR>& presentation_modes) {
        // Look for Mailbox presentation mode
        for (const auto& presentation_mode : presentation_modes) {
            if (presentation_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return presentation_mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR; // allways avaible by vulkan
    }

#pragma endregion auxiliar

    SetupSwapchain SwapChain::setup_params() {

        SetupSwapchain setup;

        // Get Swap Chain details so we cam pick best setting
        SwapChainDetails swapchain_details = VulkanContext::get_swap_chain_details(ctx_->physical, ctx_->surface);

        setup.currentTransform = swapchain_details.surfaceCapabilities.currentTransform;

        // Find optimal surface value for our swap chain,  Store for late reference
        setup.surfaceFormat = SwapChain::choose_best_surface_format(swapchain_details.formats);
        setup.presentMode = SwapChain::choose_best_presentation_mode(swapchain_details.presentationModes);
        setup.extent = choose_swap_extent(swapchain_details.surfaceCapabilities);

        // how many images are in the swap chain? Get 1 more than the minimum to allow triple buffering
        setup.imageCount = swapchain_details.surfaceCapabilities.minImageCount + 1;

        // If imagecount higher than max the clamp down to max
        // If 0, then limitless
        if (swapchain_details.surfaceCapabilities.maxImageCount > 0 &&
            swapchain_details.surfaceCapabilities.maxImageCount < setup.imageCount) {
            setup.imageCount = swapchain_details.surfaceCapabilities.maxImageCount;
        }

        // If Graphics and Presentation families are diferent, the swapchain must let images ge shared between families
        // indices.graphicsFamily == indices.presentationFamily
        setup.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (ctx_->queueFamilyIndices.graphicsFamily != ctx_->queueFamilyIndices.presentationFamily) {
            setup.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            setup.queueFamilyIndices.push_back(ctx_->queueFamilyIndices.graphicsFamily);
            setup.queueFamilyIndices.push_back(ctx_->queueFamilyIndices.presentationFamily);
        } else {
            setup.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            setup.queueFamilyIndices.push_back(ctx_->queueFamilyIndices.graphicsFamily);
        }

        return setup;
    }

} // namespace ce
