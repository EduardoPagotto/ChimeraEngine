#include "cevk_engine/ScreenVK.hpp"

namespace ce {

    ScreenVK::ScreenVK(std::shared_ptr<VulkanContext> ctx) : ctx(ctx) {
        this->swapchain.init(ctx);

        this->cmdBuffers.resize(this->swapchain.getSwapchainResSize());
        for (size_t i = 0; i < this->swapchain.getSwapchainResSize(); i++) {
            this->cmdBuffers[i] = CmdBuffer();
            this->cmdBuffers[i].init(this->ctx->logical, this->ctx->commandPool);
        }

        this->syncs.resize(ce::MAX_FRAME_DRAWS);
        for (size_t i = 0; i < ce::MAX_FRAME_DRAWS; i++) {
            this->syncs[i] = ce::Sync();
            this->syncs[i].init(this->ctx->logical);
        }
    }

    ScreenVK::~ScreenVK() {
        // Wait until no action being run on device before destroying
        vkDeviceWaitIdle(ctx->logical);

        for (size_t i = 0; i < this->syncs.size(); i++) {
            this->syncs[i].destroy();
        }

        for (size_t i = 0; i < cmdBuffers.size(); i++) {
            cmdBuffers[i].destroy();
        }
    }

    void ScreenVK::before() {}

    void ScreenVK::after() {}
    void ScreenVK::toggleFullScreen() {}
    void ScreenVK::reshape(int _width, int _height) {}
    const int ScreenVK::getWidth() const { return 0; }
    const int ScreenVK::getHeight() const { return 0; }

} // namespace ce
