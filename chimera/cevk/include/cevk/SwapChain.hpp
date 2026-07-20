#pragma once

#include "Image.hpp"
#include "cevk.hpp"
#include <memory>
#include <vector>

namespace ce {

    class SwapChain {
      public:
        explicit SwapChain(BaseVK* pBVK);
        virtual ~SwapChain();

        VkExtent2D& getExtent() { return this->extent; }
        std::vector<std::shared_ptr<Image>>& getImages() { return this->images; }
        VkFormat& getImageFormat() { return this->imageFormat; }
        std::vector<VkFramebuffer>& getSwapChainFrameBuffers() { return this->swapChainFrameBuffers; }

        void createFramebuffers(VkRenderPass& renderPass);
        void createDepthBufferImage();

        void sendImageToScreen(VkQueue pQueue, VkSemaphore signal, uint32_t& imageIndex);

        uint32_t acquireNextImage(VkSemaphore& waitImage) {
            uint32_t imageIndex;
            vkAcquireNextImageKHR(logical, this->swapchain, std::numeric_limits<uint64_t>::max(), waitImage,
                                  VK_NULL_HANDLE, &imageIndex);

            return imageIndex;
        }

      private:
        VkSwapchainKHR swapchain;
        VkFormat imageFormat;
        VkExtent2D extent;
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice logical{VK_NULL_HANDLE};

        SDL_Window* window{nullptr};

        std::vector<std::shared_ptr<Image>> images;
        std::shared_ptr<Image> depthBufferObject;
        std::vector<VkFramebuffer> swapChainFrameBuffers;

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

        static VkSurfaceFormatKHR ChooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        static VkPresentModeKHR ChooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);
    };
} // namespace ce
