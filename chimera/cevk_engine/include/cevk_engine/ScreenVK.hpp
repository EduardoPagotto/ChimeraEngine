#pragma once
#include "cevk/Frame.hpp"
#include "cevk/RenderPass.hpp"
#include "cevk/SwapChain.hpp"
#include "cevk/VulkanContext.hpp"
#include "cevk_infra/IScr.hpp"
#include <SDL3/SDL.h>
#include <memory>

namespace ce {

    /// @brief Canva Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260907
    class ScreenVK : public IScr {

      public:
        explicit ScreenVK(std::shared_ptr<VulkanContext> ctx);
        virtual ~ScreenVK();

        virtual void before() override;
        virtual void after() override;
        virtual void toggleFullScreen() override;
        virtual void reshape(int _width, int _height) override;
        virtual uint32_t getWidth() const override { return this->swapchain.getExtent().width; }
        virtual uint32_t getHeight() const override { return this->swapchain.getExtent().height; }

      private:
        std::shared_ptr<VulkanContext> ctx;
        bool fullscreen{false};

      public:
        std::vector<VkClearValue> clearValues;
        RenderPass renderPass;
        SwapChain swapchain;
        std::vector<Frame> frames;

        int currentFrame{0};
    };
} // namespace ce
