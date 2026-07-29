#pragma once

#include "Image.hpp"
#include "VulkanContext.hpp"
#include <memory>
#include <vector>

namespace ce {

    class SwapChain {
      public:
        explicit SwapChain(VulkanContext& context);
        virtual ~SwapChain();

        void sendImageToScreen(VkQueue pQueue, VkSemaphore signal, uint32_t& imageIndex);
        uint32_t acquireNextImage(VkSemaphore& waitImage, VkRenderPassBeginInfo* r);

        VkFormat& getImageFormat() { return this->imageFormat; }
        VkRenderPass& getRenderPass() { return renderPass; }
        VkExtent2D& getExtent() { return this->extent; }
        std::vector<std::shared_ptr<Image>>& getImages() { return this->images; }
        std::vector<VkFramebuffer>& getSwapChainFrameBuffers() { return this->frameBuffers; }

      private:
        void createDepthBufferImage();
        void createFramebuffers(VkRenderPass& renderPass);
        void createRenderPass(const VkFormat& format);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

        static VkSurfaceFormatKHR ChooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        static VkPresentModeKHR ChooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);

        VkFormat imageFormat;
        VkExtent2D extent;
        VkRect2D renderArea;

        VulkanContext& context;
        VkSwapchainKHR swapchain{VK_NULL_HANDLE};
        VkRenderPass renderPass{VK_NULL_HANDLE};

        std::shared_ptr<Image> depthBufferImg;
        std::vector<std::shared_ptr<Image>> images;
        std::vector<VkFramebuffer> frameBuffers;
        std::vector<VkClearValue> clearValues;
    };
} // namespace ce
