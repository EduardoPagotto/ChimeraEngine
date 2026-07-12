#pragma once

#include "Buffers.hpp"
#include <memory>
#include <vector>

namespace ce {

    class SwapChain {
      public:
        explicit SwapChain(BaseVK* pBVK);
        virtual ~SwapChain();

        VkSwapchainKHR& getKHR() { return this->swapchain; }
        VkExtent2D& getExtent() { return this->extent; }
        std::vector<std::shared_ptr<ImageObject>>& getImages() { return this->images; }
        VkFormat& getImageFormat() { return this->imageFormat; }
        std::vector<VkFramebuffer>& getSwapChainFrameBuffers() { return this->swapChainFrameBuffers; }

        void createFramebuffers(VkRenderPass& renderPass);
        void createDepthBufferImage();

        void sendImageToScreen(VkQueue pQueue, VkSemaphore signal, uint32_t& imageIndex);

      private:
        VkSwapchainKHR swapchain;
        VkFormat imageFormat;
        VkExtent2D extent;
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice logical{VK_NULL_HANDLE};

        SDL_Window* window{nullptr};

        std::vector<std::shared_ptr<ImageObject>> images;
        std::shared_ptr<ImageObject> depthBufferObject;
        std::vector<VkFramebuffer> swapChainFrameBuffers;

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

        static VkSurfaceFormatKHR ChooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        static VkPresentModeKHR ChooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);
    };
} // namespace ce
