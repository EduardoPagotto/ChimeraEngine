#pragma once

#include "Image.hpp"
#include "VulkanContext.hpp"
#include <memory>

namespace ce {

    class DepthBufferImage {
      public:
        explicit DepthBufferImage() = default;

        explicit DepthBufferImage(std::shared_ptr<VulkanContext> ctx, const VkExtent2D& extent) { init(ctx, extent); }

        virtual ~DepthBufferImage() { destroy(); }

        // Movimentação permitida para transferência de escopo
        DepthBufferImage(DepthBufferImage&& other) noexcept { *this = std::move(other); }

        // Proibir cópia (Padrão RAII)
        DepthBufferImage(const DepthBufferImage&) = delete;
        DepthBufferImage& operator=(const DepthBufferImage&) = delete;

        DepthBufferImage& operator=(DepthBufferImage&& other) noexcept {
            if (this != &other) {
                destroy();
                depth_buffer_img_ = other.depth_buffer_img_;
                other.depth_buffer_img_ = nullptr;
            }
            return *this;
        }

        void destroy() {
            if (depth_buffer_img_) {
                depth_buffer_img_.reset();
            }
        }

        void init(std::shared_ptr<VulkanContext> ctx, const VkExtent2D& extent) {

            if (depth_buffer_img_) {
                destroy();
            }

            // Get suported format for depth buffer
            VkFormat depth_format = VulkanContext::choose_supported_format(
                ctx->physical,
                {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT}, // Formats
                VK_IMAGE_TILING_OPTIMAL,                                                           // Tilling
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);                                   // Depth

            // Create Depth Buffer Image
            depth_buffer_img_ = std::make_shared<Image>(ctx->physical, ctx->logical);
            depth_buffer_img_->createImage(extent.width, extent.height, depth_format, VK_IMAGE_TILING_OPTIMAL,
                                           VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            // Create Depth Buffer Image View
            depth_buffer_img_->createImageView(VK_IMAGE_ASPECT_DEPTH_BIT);
        }

        VkImageView& get_image_view() { return depth_buffer_img_->getImageView(); }

      private:
        std::shared_ptr<Image> depth_buffer_img_{nullptr};
    };
} // namespace ce
