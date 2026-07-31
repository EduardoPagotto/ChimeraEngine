#include "cevk_engine/ScreenVK.hpp"

namespace ce {

    ScreenVK::ScreenVK(VulkanContext& context) : context(context) {
        this->swapchain = std::make_shared<SwapChain>(context);

        this->cmdBuffers.resize(this->swapchain->getSwapChainFrameBuffers().size());
        for (size_t i = 0; i < this->swapchain->getSwapChainFrameBuffers().size(); i++) {
            this->cmdBuffers[i] = CmdBuffer();
            this->cmdBuffers[i].init(this->context.logical, this->context.commandPool);
        }

        this->syncs.resize(ce::MAX_FRAME_DRAWS);
        for (size_t i = 0; i < ce::MAX_FRAME_DRAWS; i++) {
            this->syncs[i] = ce::Sync();
            this->syncs[i].init(this->context.logical);
        }
    }

    ScreenVK::~ScreenVK() {
        // Wait until no action being run on device before destroying
        vkDeviceWaitIdle(context.logical);

        for (size_t i = 0; i < this->syncs.size(); i++) {
            this->syncs[i].destroy();
        }

        for (size_t i = 0; i < cmdBuffers.size(); i++) {
            cmdBuffers[i].destroy();
        }

        this->swapchain.reset();
    }

    void ScreenVK::before() {}
    void ScreenVK::after() {}
    void ScreenVK::toggleFullScreen() {}
    void ScreenVK::reshape(int _width, int _height) {}
    const int ScreenVK::getWidth() const { return 0; }
    const int ScreenVK::getHeight() const { return 0; }

} // namespace ce
