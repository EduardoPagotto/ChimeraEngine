#include "cevk_engine/ScreenVK.hpp"

namespace ce {

    ScreenVK::ScreenVK(std::shared_ptr<VulkanContext> ctx) : ctx(ctx) {}
    ScreenVK::~ScreenVK() {}

    void ScreenVK::before() {}

    void ScreenVK::after() {}
    void ScreenVK::toggleFullScreen() {}
    void ScreenVK::reshape(int _width, int _height) {}
    int ScreenVK::getWidth() const { return 0; }
    int ScreenVK::getHeight() const { return 0; }

} // namespace ce
