#pragma once

#include "Image.hpp"
#include "VulkanContext.hpp"
#include <memory>
#include <vector>

// Estrutura para os recursos individuais de cada imagem da Swapchain
struct SwapchainImageResource {
    VkImage image = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
    VkFramebuffer framebuffer = VK_NULL_HANDLE; // Gerenciado via ciclo da Swapchain
    VkFence inFlightFence = VK_NULL_HANDLE;     // Rastreia se esta imagem específica está em uso

    // RAII: A Swapchain possui as VkImages, então destruímos apenas a View e liberamos a Fence externa se necessário
    void cleanup(VkDevice device) {
        if (framebuffer) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
            framebuffer = VK_NULL_HANDLE;
        }
        if (imageView) {
            vkDestroyImageView(device, imageView, nullptr);
            imageView = VK_NULL_HANDLE;
        }
        // Nota: As Fences de imagem são referências apontando para as Fences do FrameData,
        // ou criadas separadamente caso queira controle individual.
    }
};

namespace ce {

    class SwapChain {
      public:
        explicit SwapChain() = default;
        virtual ~SwapChain() { this->destroy(); }

        void init(std::shared_ptr<VulkanContext> ctx);
        void destroy();

        void sendImageToScreen(VkQueue pQueue, VkSemaphore signal, uint32_t& imageIndex);
        uint32_t acquireNextImage(VkSemaphore& waitImage, VkRenderPassBeginInfo* r);

        VkFormat& getImageFormat() { return this->imageFormat; }
        VkRenderPass& getRenderPass() { return renderPass; }
        VkExtent2D& getExtent() { return this->extent; }
        size_t getSwapchainResSize() const { return this->swapchainRes.size(); }
        SwapchainImageResource& getSwapchainRes(size_t index) { return this->swapchainRes[index]; }

      private:
        void createDepthBufferImage();
        void createRenderPass(const VkFormat& format);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

        static VkSurfaceFormatKHR ChooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        static VkPresentModeKHR ChooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);

        VkFormat imageFormat;
        VkExtent2D extent;
        VkRect2D renderArea;

        VkSwapchainKHR swapchain{VK_NULL_HANDLE};
        VkRenderPass renderPass{VK_NULL_HANDLE};

        std::shared_ptr<VulkanContext> ctx;
        std::shared_ptr<Image> depthBufferImg;

        std::vector<SwapchainImageResource> swapchainRes;
        std::vector<VkClearValue> clearValues;
    };
} // namespace ce
