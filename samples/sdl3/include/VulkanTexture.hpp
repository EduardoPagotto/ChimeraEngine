#pragma once
#include "Buffers.hpp"
#include "CmdBuffer.hpp"
#include "Image.hpp"
#include "VulkanContext.hpp"
#include <SDL3_image/SDL_image.h>
#include <format>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace ce {

    class VulkanTexture {

      public:
        VulkanTexture(std::shared_ptr<Image> texImg) : texImg(texImg), index(nextTextureIndex++) {}
        ~VulkanTexture() { texImg.reset(); }

        // Factory pattern exigido pelo AssetManager
        static std::shared_ptr<VulkanTexture> create(std::shared_ptr<VulkanContext> ctx, const std::string& filePath) {

            SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
            if (!loadedSurface) {
                throw std::runtime_error(std::format("{}", SDL_GetError()));
            }

            SDL_Surface* surface = SDL_ConvertSurface(loadedSurface, SDL_PIXELFORMAT_ABGR8888);
            SDL_DestroySurface(loadedSurface); // Libera o original intermediário

            VkDeviceSize imageSize = surface->w * surface->h * 4;
            uint32_t texWidth = surface->w;
            uint32_t texHeight = surface->h;

            // Create staging buffer to hold load data, redy to copy device
            Buffer imageStagingBuffer(ctx->physical, ctx->logical);
            imageStagingBuffer.create(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            // copy image data to staging buffer
            imageStagingBuffer.mapper(surface->pixels);

            // Os pixels já estão na memória do Vulkan. Podemos destruir a superfície SDL.
            SDL_DestroySurface(surface);

            // create image to hold final texture
            std::shared_ptr<Image> texImg = std::make_shared<Image>(ctx->physical, ctx->logical);

            texImg->createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
                                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            // COPY DATA TO IMAGE
            // Transition image to be DST for copy operation
            aux::TransitionImageLayout(ctx->logical, ctx->graphicsQueue, ctx->commandPool, texImg->getImage(),
                                       VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            // Copy image data
            aux::CopyImageBuffer(ctx->logical, ctx->graphicsQueue, ctx->commandPool, imageStagingBuffer.get(),
                                 texImg->getImage(), texWidth, texHeight);

            // Transition image to be shader readable for shader
            aux::TransitionImageLayout(ctx->logical, ctx->graphicsQueue, ctx->commandPool, texImg->getImage(),
                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

            texImg->createImageView(VK_IMAGE_ASPECT_COLOR_BIT);
            return std::make_shared<VulkanTexture>(texImg);
        }

        // VkImageView& getImageView() const { return this->texImg->getImageView(); }
        // VkImage& getImage() const { return this->texImg->getImage(); }
        std::shared_ptr<Image> get() { return this->texImg; }

        void clearBindlessIndex() { this->delta = index; }

        const uint32_t getBindlessIndex() const { return (index - delta); }
        const uint32_t getIndex() const { return this->index; }
        inline static void ResetIndex() { nextTextureIndex = 0; }

      private:
        std::shared_ptr<Image> texImg;
        uint32_t index{0};
        uint32_t delta{0};

        inline static uint32_t nextTextureIndex = 0;
    };
} // namespace ce
