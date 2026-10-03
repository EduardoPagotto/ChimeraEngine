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
            if (framebuffer != VK_NULL_HANDLE) {
                vkDestroyFramebuffer(device, framebuffer, nullptr);
                framebuffer = VK_NULL_HANDLE;
            }
            if (imageView != VK_NULL_HANDLE) {
                vkDestroyImageView(device, imageView, nullptr);
                imageView = VK_NULL_HANDLE;
            }
            // Nota: As Fences de imagem são referências apontando para as Fences do FrameData,
            // ou criadas separadamente caso queira controle individual.
        }

        void create(VkDevice device, VkImage image, VkRenderPass renderpass, const VkExtent2D& extent, VkFormat& format,
                    VkImageView depth_buffer) {

            this->image = image;
            inFlightFence = VK_NULL_HANDLE;
            VkImageViewCreateInfo view_info{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                            .image = image,
                                            .viewType = VK_IMAGE_VIEW_TYPE_2D,
                                            .format = format,
                                            .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                                                 .baseMipLevel = 0,
                                                                 .levelCount = 1,
                                                                 .baseArrayLayer = 0,
                                                                 .layerCount = 1}};

            vkCreateImageView(device, &view_info, nullptr, &imageView);

            // Create framebuffer usinf color map and depth buffer
            std::vector<VkImageView> attachments;
            attachments.push_back(imageView);
            if (depth_buffer != VK_NULL_HANDLE) {
                attachments.push_back(depth_buffer); // order important same as upper
            }

            VkFramebufferCreateInfo framebuffer_info{
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .renderPass = renderpass,
                .attachmentCount = static_cast<uint32_t>(attachments.size()), //
                .pAttachments = attachments.data(), // List of attachments (1:1 with Render Pass)
                .width = extent.width,
                .height = extent.height,
                .layers = 1};

            vkCreateFramebuffer(device, &framebuffer_info, nullptr, &framebuffer);
        }

        __attribute__((always_inline)) void sync_img(VkDevice device, VkFence frame_fence) {
            // Se a imagem real adquirida ainda estiver sendo usada por algum frame virtual anterior, aguarde.
            if (inFlightFence != VK_NULL_HANDLE) {
                vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
            }

            // Mapeia a Fence do frame virtual atual para esta imagem da swapchain.
            inFlightFence = frame_fence;
        }
    };

    // Classe RAII para os dados da Swapchain
    class SwapchainData {

      public:
        SwapchainData() = default;
        SwapchainData(VkDevice device, VkSwapchainKHR old_swapchain = VK_NULL_HANDLE)
            : device_(device), swapchain(old_swapchain) {}

        ~SwapchainData() { destroy(); }

        // Movimentação permitida para transferência de escopo
        SwapchainData(SwapchainData&& other) noexcept { *this = std::move(other); }

        SwapchainData& operator=(SwapchainData&& other) noexcept {
            if (this != &other) {
                destroy();
                device_ = other.device_;
                swapchain = other.swapchain;
                images = std::move(other.images);

                other.swapchain = VK_NULL_HANDLE;
                other.device_ = VK_NULL_HANDLE;
            }
            return *this;
        }

        // Proibir cópia (Padrão RAII)
        SwapchainData(const SwapchainData&) = delete;
        SwapchainData& operator=(const SwapchainData&) = delete;

        void destroy() {
            if (device_ != VK_NULL_HANDLE) {
                for (auto& img_res : images) {
                    img_res.cleanup(device_);
                }
                images.clear();

                if (swapchain != VK_NULL_HANDLE) {
                    vkDestroySwapchainKHR(device_, swapchain, nullptr);
                    swapchain = VK_NULL_HANDLE;
                }
                device_ = VK_NULL_HANDLE;
            }
        }

      private:
        VkDevice device_{VK_NULL_HANDLE};

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
        virtual ~SwapChain() { destroy(); }

        void init(std::shared_ptr<VulkanContext> ctx, VkRenderPass render_pass, bool depth_buffer_enable = true);
        void destroy();

        std::pair<uint32_t, SwapchainImageResource&> acquire_next_image(VkFence& in_flight_fence,
                                                                        VkSemaphore& wait_image);

        VkFormat& get_image_format() { return surface_format_.format; }
        VkSwapchainKHR get_swapchain() const { return swapchain_data_.swapchain; }
        const VkExtent2D& get_extent() const { return extent_; }
        size_t get_swapchain_res_size() const { return swapchain_data_.images.size(); }
        SwapchainImageResource& get_swapchain_res(size_t index) { return swapchain_data_.images[index]; }

        static VkSurfaceFormatKHR choose_best_surface_format(const std::vector<VkSurfaceFormatKHR>& formats);

        VkRect2D& get_render_area() { return render_area_; }

        void recreate_swapchain();

      private:
        void create_swapchain(bool depth_buffer_enable, bool rebuild);

        SetupSwapchain setup_params();

        VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& surface_capabilities);

        static VkPresentModeKHR choose_best_presentation_mode(const std::vector<VkPresentModeKHR>& presentation_modes);

        VkSurfaceFormatKHR surface_format_;
        VkExtent2D extent_;
        VkRect2D render_area_;

        SwapchainData swapchain_data_;
        VkRenderPass renderpass_{VK_NULL_HANDLE};

        std::shared_ptr<VulkanContext> ctx_{VK_NULL_HANDLE};
        std::shared_ptr<DepthBufferImage> depth_buffer_{nullptr};
    };
} // namespace ce
