#pragma once

#include "Image.hpp"
#include "VulkanContext.hpp"
#include <memory>

namespace ce {

    class DepthBufferImage {
      public:
        explicit DepthBufferImage() = default;

        explicit DepthBufferImage(std::shared_ptr<VulkanContext> ctx, const VkExtent2D& extent) {
            this->init(ctx, extent);
        }

        virtual ~DepthBufferImage() { this->destroy(); }

        // Movimentação permitida para transferência de escopo
        DepthBufferImage(DepthBufferImage&& other) noexcept { *this = std::move(other); }

        // Proibir cópia (Padrão RAII)
        DepthBufferImage(const DepthBufferImage&) = delete;
        DepthBufferImage& operator=(const DepthBufferImage&) = delete;

        DepthBufferImage& operator=(DepthBufferImage&& other) noexcept {
            if (this != &other) {
                destroy();
                this->depthBufferImg = other.depthBufferImg;
                other.depthBufferImg = nullptr;
            }
            return *this;
        }

        void destroy() {
            if (this->depthBufferImg) {
                this->depthBufferImg.reset();
            }
        }

        void init(std::shared_ptr<VulkanContext> ctx, const VkExtent2D& extent) {

            if (depthBufferImg) {
                this->destroy();
            }

            // Get suported format for depth buffer
            VkFormat depthFormat = VulkanContext::ChooseSupportedFormat(
                ctx->physical,
                {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT}, // Formats
                VK_IMAGE_TILING_OPTIMAL,                                                           // Tilling
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);                                   // Depth

            // Create Depth Buffer Image
            this->depthBufferImg = std::make_shared<Image>(ctx->physical, ctx->logical);
            this->depthBufferImg->createImage(extent.width, extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
                                              VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            // Create Depth Buffer Image View
            this->depthBufferImg->createImageView(VK_IMAGE_ASPECT_DEPTH_BIT);
        }

        VkImageView& getImageView() { return depthBufferImg->getImageView(); }

      private:
        std::shared_ptr<Image> depthBufferImg{nullptr};
    };
} // namespace ce
