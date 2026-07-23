#pragma once

#include "Image.hpp"
#include "cevk.hpp"
#include <memory>
#include <vector>

namespace ce {

    class SwapChain {
      public:
        explicit SwapChain(BaseVK* pBVK, QueueFamilyIndices& indices);
        virtual ~SwapChain();

        VkExtent2D& getExtent() { return this->extent; }
        std::vector<std::shared_ptr<Image>>& getImages() { return this->images; }
        VkFormat& getImageFormat() { return this->imageFormat; }
        std::vector<VkFramebuffer>& getSwapChainFrameBuffers() { return this->frameBuffers; }

        void sendImageToScreen(VkQueue pQueue, VkSemaphore signal, uint32_t& imageIndex);

        uint32_t acquireNextImage(VkSemaphore& waitImage, VkRenderPassBeginInfo* r) {
            uint32_t imageIndex;
            vkAcquireNextImageKHR(logical, this->swapchain, std::numeric_limits<uint64_t>::max(), waitImage,
                                  VK_NULL_HANDLE, &imageIndex);

            r->sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            r->renderPass = this->renderPass;                               // Render pass to begin
            r->framebuffer = this->frameBuffers[imageIndex];                //
            r->renderArea = this->renderArea;                               //
            r->clearValueCount = static_cast<uint32_t>(clearValues.size()); //
            r->pClearValues = clearValues.data();                           // List of clear values

            return imageIndex;
        }

        VkRenderPass& getRenderPass() { return renderPass; }

      private:
        void createDepthBufferImage();
        void createFramebuffers(VkRenderPass& renderPass);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

        static VkSurfaceFormatKHR ChooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        static VkPresentModeKHR ChooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);

        void createRenderPass(const VkFormat& format);

        VkFormat imageFormat;
        VkExtent2D extent;
        VkRect2D renderArea;

        VkSwapchainKHR swapchain{VK_NULL_HANDLE};
        VkRenderPass renderPass{VK_NULL_HANDLE};
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice logical{VK_NULL_HANDLE};
        SDL_Window* window{nullptr};

        std::shared_ptr<Image> depthBufferImg;
        std::vector<std::shared_ptr<Image>> images;
        std::vector<VkFramebuffer> frameBuffers;
        std::vector<VkClearValue> clearValues;
    };
} // namespace ce
