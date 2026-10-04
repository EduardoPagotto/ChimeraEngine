#include "cevk_engine/CanvaVK.hpp"

namespace ce {

    CanvaVK::CanvaVK(const std::string& name, const int& width, const int& height) {

        this->ctx_ = std::make_shared<VulkanContext>();
        this->ctx_->create_window(name, width, height);

        // clear colour
        this->clearValues.resize(2);
        // this->clearValues[0].color = {{0.6F, 0.65F, 0.4F, 1.0F}};
        this->clearValues[0].color = {{0.0F, 0.0F, 0.0F, 1.0F}};
        this->clearValues[1].depthStencil.depth = 1.0F;

        // Get Swap Chain details so we cam pick best setting
        SwapChainDetails swapchain_details = VulkanContext::get_swap_chain_details(ctx_->physical, ctx_->surface);
        this->renderPass.init(ctx_, SwapChain::choose_best_surface_format(swapchain_details.formats).format);

        this->swapchain.init(ctx_, this->renderPass.get_render_pass());

        this->frames.resize(ce::max_frame_draws);
        for (size_t i = 0; i < ce::max_frame_draws; i++) {
            this->frames[i] = ce::Frame();
            this->frames[i].init(this->ctx_->logical, this->ctx_->queueFamilyIndices.graphicsFamily);
        }
    }
    CanvaVK::~CanvaVK() {
        this->swapchain.destroy();
        this->renderPass.destroy();
    }

    void CanvaVK::before() {}

    void CanvaVK::after() {

        ce::Frame& frame = this->frames[this->currentFrame];

        // -- PRESENT RENDERED IMAGE TO SCREEN --
        VkResult result = ce::RenderPass::send_image_to_screen(ctx_->presentationQueue, frame.renderFinishedSemaphore,
                                                               this->swapchain.get_swapchain(), this->indexFrame);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) { //|| framebufferResized
            SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "resized (%d)...", result);
            // framebufferResized = false;
            this->swapchain.recreate_swapchain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present Swapchain!");
        }

        // Get next frame
        this->currentFrame = (this->currentFrame + 1) % ce::max_frame_draws;
        // AHHHH!!!!!! ugly!!!!! this is complete wrong, find what missmatch sYncs!!!
        if (this->currentFrame == (ce::max_frame_draws - 1)) {
            vkDeviceWaitIdle(ctx_->logical);
        }
    }

    void CanvaVK::toggle_fullscreen() {
        SDL_SetWindowFullscreen(ctx_->window, !this->fullscreen_);
        this->fullscreen_ = !this->fullscreen_;
    }

    void CanvaVK::reshape(int width, int height) {
        eventReShape = true;
        this->swapchain.recreate_swapchain();
    }

    std::pair<uint32_t, VkRenderPassBeginInfo> CanvaVK::next_image_renderpass() {
        // -- GET NEXT IMAGE --
        ce::Frame& frame = this->frames[this->currentFrame];

        // Get index of next image to be draw to, execute sincronization
        auto [imageIndex, swapchainRes] =
            this->swapchain.acquire_next_image(frame.inFlightFence, frame.imageAvailableSemaphore);

        this->indexFrame = imageIndex;

        return {imageIndex,
                {
                    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                    .renderPass = this->renderPass.get_render_pass(),                   // Render pass to begin
                    .framebuffer = swapchainRes.framebuffer,                            //
                    .renderArea = this->swapchain.get_render_area(),                    //
                    .clearValueCount = static_cast<uint32_t>(this->clearValues.size()), //
                    .pClearValues = this->clearValues.data(),                           // List of clear values
                }};
    }

} // namespace ce
