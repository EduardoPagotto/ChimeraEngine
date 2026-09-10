#include "cevk_engine/ScreenVK.hpp"
#include "cevk/CmdRender.hpp"

namespace ce {

    ScreenVK::ScreenVK(std::shared_ptr<VulkanContext> ctx) : ctx(ctx) {

        // clear colour
        this->clearValues.resize(2);
        // this->clearValues[0].color = {{0.6F, 0.65F, 0.4F, 1.0F}};
        this->clearValues[0].color = {{0.0F, 0.0F, 0.0F, 1.0F}};
        this->clearValues[1].depthStencil.depth = 1.0F;

        // Get Swap Chain details so we cam pick best setting
        SwapChainDetails swapchainDetails = VulkanContext::GetSwapChainDetails(ctx->physical, ctx->surface);
        this->renderPass.init(ctx, SwapChain::ChooseBestSurfaceFormat(swapchainDetails.formats).format);

        this->swapchain.init(ctx, this->renderPass.getRenderPass());

        this->frames.resize(ce::MAX_FRAME_DRAWS);
        for (size_t i = 0; i < ce::MAX_FRAME_DRAWS; i++) {
            this->frames[i] = ce::Frame();
            this->frames[i].init(this->ctx->logical, this->ctx->queueFamilyIndices.graphicsFamily);
        }
    }
    ScreenVK::~ScreenVK() {
        this->swapchain.destroy();
        this->renderPass.destroy();
    }

    void ScreenVK::before() {
        // // -- GET NEXT IMAGE --
        // ce::Frame& frame = this->frames[this->currentFrame];

        // // Get index of next image to be draw to, execute sincronization
        // auto [imageIndex, swapchainRes] =
        //     this->swapchain.acquireNextImage(frame.inFlightFence, frame.imageAvailableSemaphore);

        // VkRenderPassBeginInfo renderPassBeginInfo = {
        //     .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        //     .renderPass = this->renderPass.getRenderPass(),               // Render pass to begin
        //     .framebuffer = swapchainRes.framebuffer,                      //
        //     .renderArea = this->swapchain.getRenderArea(),                //
        //     .clearValueCount = static_cast<uint32_t>(clearValues.size()), //
        //     .pClearValues = clearValues.data(),                           // List of clear values
        // };

        // ce::CmdRender cmd;
        // cmd.begin(frame.commandBuffer, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, renderPassBeginInfo,
        //           this->graphicPipeline->get());
    }

    void ScreenVK::after() {

        // final
        // Get next frame
        this->currentFrame = (this->currentFrame + 1) % ce::MAX_FRAME_DRAWS;
        // AHHHH!!!!!! ugly!!!!! this is complete wrong, find what missmatch sYncs!!!
        if (this->currentFrame == (ce::MAX_FRAME_DRAWS - 1)) {
            vkDeviceWaitIdle(ctx->logical);
        }
    }

    void ScreenVK::toggleFullScreen() {
        SDL_SetWindowFullscreen(ctx->window, !this->fullscreen);
        this->fullscreen = !this->fullscreen;
    }

    void ScreenVK::reshape(int _width, int _height) {}

} // namespace ce
