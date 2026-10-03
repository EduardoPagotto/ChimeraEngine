#include "cevk_engine/CanvaVK.hpp"

namespace ce {

    CanvaVK::CanvaVK(const std::string& name, const int& width, const int& height) {

        this->ctx_ = std::make_shared<VulkanContext>();
        this->ctx_->createWindow(name, width, height);

        // clear colour
        this->clearValues.resize(2);
        // this->clearValues[0].color = {{0.6F, 0.65F, 0.4F, 1.0F}};
        this->clearValues[0].color = {{0.0F, 0.0F, 0.0F, 1.0F}};
        this->clearValues[1].depthStencil.depth = 1.0F;

        // Get Swap Chain details so we cam pick best setting
        SwapChainDetails swapchain_details = VulkanContext::GetSwapChainDetails(ctx_->physical, ctx_->surface);
        this->renderPass.init(ctx_, SwapChain::ChooseBestSurfaceFormat(swapchain_details.formats).format);

        this->swapchain.init(ctx_, this->renderPass.getRenderPass());

        this->frames.resize(ce::MAX_FRAME_DRAWS);
        for (size_t i = 0; i < ce::MAX_FRAME_DRAWS; i++) {
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
        VkResult result = ce::RenderPass::SendImageToScreen(ctx_->presentationQueue, frame.renderFinishedSemaphore,
                                                            this->swapchain.getSwapchain(), this->indexFrame);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) { //|| framebufferResized
            SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "resized (%d)...", result);
            // framebufferResized = false;
            this->swapchain.recreateSwapchain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present Swapchain!");
        }

        // Get next frame
        this->currentFrame = (this->currentFrame + 1) % ce::MAX_FRAME_DRAWS;
        // AHHHH!!!!!! ugly!!!!! this is complete wrong, find what missmatch sYncs!!!
        if (this->currentFrame == (ce::MAX_FRAME_DRAWS - 1)) {
            vkDeviceWaitIdle(ctx_->logical);
        }
    }

    void CanvaVK::toggleFullScreen() {
        SDL_SetWindowFullscreen(ctx_->window, !this->fullscreen_);
        this->fullscreen_ = !this->fullscreen_;
    }

    void CanvaVK::reshape(int width, int height) {
        eventReShape = true;
        this->swapchain.recreateSwapchain();
    }

    std::pair<uint32_t, VkRenderPassBeginInfo> CanvaVK::next_image_renderpass() {
        // -- GET NEXT IMAGE --
        ce::Frame& frame = this->frames[this->currentFrame];

        // Get index of next image to be draw to, execute sincronization
        auto [imageIndex, swapchainRes] =
            this->swapchain.acquireNextImage(frame.inFlightFence, frame.imageAvailableSemaphore);

        this->indexFrame = imageIndex;

        return {imageIndex,
                {
                    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                    .renderPass = this->renderPass.getRenderPass(),                     // Render pass to begin
                    .framebuffer = swapchainRes.framebuffer,                            //
                    .renderArea = this->swapchain.getRenderArea(),                      //
                    .clearValueCount = static_cast<uint32_t>(this->clearValues.size()), //
                    .pClearValues = this->clearValues.data(),                           // List of clear values
                }};
    }

} // namespace ce
