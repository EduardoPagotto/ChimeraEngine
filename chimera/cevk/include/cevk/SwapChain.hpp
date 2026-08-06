#pragma once

#include "DepthBufferImage.hpp"
#include "VulkanContext.hpp"
#include <memory>
#include <vector>

namespace ce {

    // Estrutura para os recursos individuais de cada imagem da Swapchain
    struct SwapchainImageResource {
        VkImage image = VK_NULL_HANDLE;
        VkImageView imageView = VK_NULL_HANDLE;
        VkFramebuffer framebuffer = VK_NULL_HANDLE; // Gerenciado via ciclo da Swapchain
        VkFence inFlightFence = VK_NULL_HANDLE;     // Rastreia se esta imagem específica está em uso

        // RAII: A Swapchain possui as VkImages, então destruímos apenas a View e liberamos a Fence externa se
        // necessário
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

    // Classe RAII para os dados da Swapchain
    class SwapchainData {

      public:
        SwapchainData() = default;
        SwapchainData(VkDevice device, VkSwapchainKHR oldSwapchain = VK_NULL_HANDLE)
            : device(device), swapchain(oldSwapchain) {}

        ~SwapchainData() { this->destroy(); }

        // Movimentação permitida para transferência de escopo
        SwapchainData(SwapchainData&& other) noexcept { *this = std::move(other); }

        SwapchainData& operator=(SwapchainData&& other) noexcept {
            if (this != &other) {
                destroy();
                this->device = other.device;
                this->swapchain = other.swapchain;
                this->images = std::move(other.images);

                other.swapchain = VK_NULL_HANDLE;
                other.device = VK_NULL_HANDLE;
            }
            return *this;
        }

        // Proibir cópia (Padrão RAII)
        SwapchainData(const SwapchainData&) = delete;
        SwapchainData& operator=(const SwapchainData&) = delete;

        void destroy() {
            if (device) {
                for (auto& imgRes : images) {
                    imgRes.cleanup(device);
                }
                images.clear();

                if (swapchain != VK_NULL_HANDLE) {
                    vkDestroySwapchainKHR(device, swapchain, nullptr);
                    swapchain = VK_NULL_HANDLE;
                }
                device = VK_NULL_HANDLE;
            }
        }

      private:
        VkDevice device{VK_NULL_HANDLE};

      public:
        VkSwapchainKHR swapchain{VK_NULL_HANDLE};
        std::vector<SwapchainImageResource> images;
    };

    struct SetupSwapchain {
        VkSurfaceFormatKHR surfaceFormat;
        VkSurfaceTransformFlagBitsKHR currentTransform;
        VkPresentModeKHR presentMode;
        VkExtent2D extent;
        uint32_t imageCount{0};
        VkSharingMode imageSharingMode;
        // uint32_t queueFamilyIndexCount{0};
        //  pQueueFamilyIndices
        std::vector<uint32_t> queueFamilyIndices;
    };

    class SwapChain {
      public:
        explicit SwapChain() = default;
        virtual ~SwapChain() { this->destroy(); }

        void init(std::shared_ptr<VulkanContext> ctx, bool depthBufferEnable = true);
        void destroy();

        void sendImageToScreen(VkQueue pQueue, VkSemaphore signal, uint32_t& imageIndex);
        uint32_t acquireNextImage(VkSemaphore& waitImage, VkRenderPassBeginInfo* r);

        VkFormat& getImageFormat() { return this->surfaceFormat.format; }
        VkRenderPass& getRenderPass() { return renderPass; }
        VkExtent2D& getExtent() { return this->extent; }
        size_t getSwapchainResSize() const { return this->swapchainData.images.size(); }
        SwapchainImageResource& getSwapchainRes(size_t index) { return this->swapchainData.images[index]; }

        // Método público para ser chamado pelo laço de eventos da janela
        void notifyResize() { framebufferResized = true; }

      private:
        void createSwapchain();
        void recreateSwapchain();
        SetupSwapchain setupParams();

        void createRenderPass(const VkFormat& format);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

        static VkSurfaceFormatKHR ChooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        static VkPresentModeKHR ChooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);

        VkSurfaceFormatKHR surfaceFormat;
        VkExtent2D extent;
        VkRect2D renderArea;

        SwapchainData swapchainData;
        VkRenderPass renderPass{VK_NULL_HANDLE};

        bool framebufferResized{false};

        std::shared_ptr<VulkanContext> ctx{VK_NULL_HANDLE};
        std::shared_ptr<DepthBufferImage> depthBuffer{nullptr};
        std::vector<VkClearValue> clearValues;
    };
} // namespace ce
