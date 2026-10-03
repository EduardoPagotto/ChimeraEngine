#pragma once
#include "cevk/Frame.hpp"
#include "cevk/RenderPass.hpp"
#include "cevk/SwapChain.hpp"
#include "cevk/VulkanContext.hpp"
#include "chimera_base/ICanva.hpp"
#include <SDL3/SDL.h>
#include <memory>

namespace ce {

    /// @brief Canva Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20260801
    /// @date 20261002
    class CanvaVK : public ICanva {

      public:
        explicit CanvaVK(const std::string& name, const int& width, const int& height);
        virtual ~CanvaVK();

        virtual void before() override;
        virtual void after() override;
        virtual void toggleFullScreen() override;
        virtual void reshape(int width, int height) override;
        virtual uint32_t getWidth() const override { return this->swapchain.getExtent().width; }
        virtual uint32_t getHeight() const override { return this->swapchain.getExtent().height; }

        std::pair<uint32_t, VkRenderPassBeginInfo> next_image_renderpass();

        std::shared_ptr<VulkanContext> ctx() { return ctx_; }

      private:
        bool fullscreen_{false};
        std::shared_ptr<VulkanContext> ctx_;

      public:
        std::vector<VkClearValue> clearValues;
        RenderPass renderPass;
        SwapChain swapchain;
        std::vector<Frame> frames;

        int currentFrame{0};
        uint32_t indexFrame{0};

        bool eventReShape{false};
    };
} // namespace ce
