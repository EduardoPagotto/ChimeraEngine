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
    /// @since 20130925
    /// @date 20260907
    class CanvaVK : public ICanva {

      public:
        explicit CanvaVK(std::shared_ptr<VulkanContext> ctx);
        virtual ~CanvaVK();

        virtual void before() override;
        virtual void after() override;
        virtual void toggleFullScreen() override;
        virtual void reshape(int _width, int _height) override;
        virtual uint32_t getWidth() const override { return this->swapchain.getExtent().width; }
        virtual uint32_t getHeight() const override { return this->swapchain.getExtent().height; }

        std::pair<uint32_t, VkRenderPassBeginInfo> nextImageRenderPass();

      private:
        std::shared_ptr<VulkanContext> ctx;
        bool fullscreen{false};

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
