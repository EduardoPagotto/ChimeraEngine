#ifndef __SWAPCHAIN_CLASS_HPP
#define __SWAPCHAIN_CLASS_HPP

#include "buffers/ImageObject.hpp"
#include "cevk.hpp"
#include <memory>
#include <vector>

namespace ce {

    class SwapChain {
      public:
        explicit SwapChain(std::shared_ptr<BaseVK> bvk);
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
        std::shared_ptr<BaseVK> bvk;
        std::vector<std::shared_ptr<ImageObject>> images;
        std::shared_ptr<ce::ImageObject> depthBufferObject;
        std::vector<VkFramebuffer> swapChainFrameBuffers;

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

        static VkSurfaceFormatKHR ChooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        static VkPresentModeKHR ChooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);
    };
} // namespace ce
#endif
