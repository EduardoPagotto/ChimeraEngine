#pragma once

#include "DepthBufferImage.hpp"
#include "VulkanContext.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    // Estrutura para os recursos individuais de cada imagem da Swapchain
    struct SwapchainImageResource {
        VkImage image{VK_NULL_HANDLE};
        VkImageView imageView{VK_NULL_HANDLE};
        VkFramebuffer framebuffer{VK_NULL_HANDLE}; // Gerenciado via ciclo da Swapchain
        VkFence inFlightFence{VK_NULL_HANDLE};     // Rastreia se esta imagem específica está em uso

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

        void create(VkDevice device, VkImage image, VkRenderPass renderpass, const VkExtent2D& extent, VkFormat& format,
                    VkImageView depthBuffer) {

            this->image = image;
            this->inFlightFence = VK_NULL_HANDLE;
            VkImageViewCreateInfo viewInfo{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                           .image = this->image,
                                           .viewType = VK_IMAGE_VIEW_TYPE_2D,
                                           .format = format,
                                           .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                                                .baseMipLevel = 0,
                                                                .levelCount = 1,
                                                                .baseArrayLayer = 0,
                                                                .layerCount = 1}};

            vkCreateImageView(device, &viewInfo, nullptr, &this->imageView);

            // Create framebuffer usinf color map and depth buffer
            std::vector<VkImageView> attachments;
            attachments.push_back(this->imageView);
            if (depthBuffer != VK_NULL_HANDLE) {
                attachments.push_back(depthBuffer); // order important same as upper
            }

            VkFramebufferCreateInfo framebufferInfo{
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .renderPass = renderpass,
                .attachmentCount = static_cast<uint32_t>(attachments.size()), //
                .pAttachments = attachments.data(), // List of attachments (1:1 with Render Pass)
                .width = extent.width,
                .height = extent.height,
                .layers = 1};

            vkCreateFramebuffer(device, &framebufferInfo, nullptr, &this->framebuffer);
        }

        __attribute__((always_inline)) inline void syncImg(VkDevice device, VkFence frameFence) {
            // Se a imagem real adquirida ainda estiver sendo usada por algum frame virtual anterior, aguarde.
            if (this->inFlightFence != VK_NULL_HANDLE) {
                vkWaitForFences(device, 1, &this->inFlightFence, VK_TRUE, UINT64_MAX);
            }

            // Mapeia a Fence do frame virtual atual para esta imagem da swapchain.
            this->inFlightFence = frameFence;
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

        void init(std::shared_ptr<VulkanContext> ctx, VkRenderPass renderPass, bool depthBufferEnable = true);
        void destroy();

        std::pair<uint32_t, SwapchainImageResource&> acquireNextImage(VkFence& inFlightFence, VkSemaphore& waitImage);

        VkFormat& getImageFormat() { return this->surfaceFormat.format; }
        VkSwapchainKHR getSwapchain() { return this->swapchainData.swapchain; }
        VkExtent2D& getExtent() { return this->extent; }
        size_t getSwapchainResSize() const { return this->swapchainData.images.size(); }
        SwapchainImageResource& getSwapchainRes(size_t index) { return this->swapchainData.images[index]; }

        // Método público para ser chamado pelo laço de eventos da janela
        void notifyResize() { framebufferResized = true; }

        static VkSurfaceFormatKHR ChooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);

        VkRect2D& getRenderArea() { return this->renderArea; }

        void recreateSwapchain();

      private:
        void createSwapchain(bool depthBufferEnable, bool rebuild);

        SetupSwapchain setupParams();

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

        static VkPresentModeKHR ChooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);

        VkSurfaceFormatKHR surfaceFormat;
        VkExtent2D extent;
        VkRect2D renderArea;

        SwapchainData swapchainData;
        VkRenderPass renderpass{VK_NULL_HANDLE};

        bool framebufferResized{false};

        std::shared_ptr<VulkanContext> ctx{VK_NULL_HANDLE};
        std::shared_ptr<DepthBufferImage> depthBuffer{nullptr};
    };
} // namespace ce
