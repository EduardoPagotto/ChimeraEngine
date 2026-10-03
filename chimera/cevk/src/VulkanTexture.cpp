#include "VulkanTexture.hpp"
#include "Buffers.hpp"
#include "CmdBuffer.hpp"
#include <format>
#include <stdexcept>

namespace ce {

    // Factory pattern exigido pelo AssetManager
    std::shared_ptr<VulkanTexture> VulkanTexture::create(std::shared_ptr<VulkanContext> ctx,
                                                         const std::string& file_path) {

        SDL_Surface* loaded_surface = IMG_Load(file_path.c_str());
        if (loaded_surface == nullptr) {
            throw std::runtime_error(std::format("{}", SDL_GetError()));
        }

        SDL_Surface* surface = SDL_ConvertSurface(loaded_surface, SDL_PIXELFORMAT_ABGR8888);
        SDL_DestroySurface(loaded_surface); // Libera o original intermediário

        VkDeviceSize image_size = static_cast<VkDeviceSize>(surface->w) * static_cast<VkDeviceSize>(surface->h) * 4;
        uint32_t tex_width = surface->w;
        uint32_t tex_height = surface->h;

        // Create staging buffer to hold load data, redy to copy device
        Buffer image_staging_buffer(ctx->physical, ctx->logical);
        image_staging_buffer.create(image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // copy image data to staging buffer
        image_staging_buffer.mapper(surface->pixels);

        // Os pixels já estão na memória do Vulkan. Podemos destruir a superfície SDL.
        SDL_DestroySurface(surface);

        // create image to hold final texture
        std::shared_ptr<Image> tex_img = std::make_shared<Image>(ctx->physical, ctx->logical);

        tex_img->createImage(tex_width, tex_height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
                             VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // COPY DATA TO IMAGE
        // Transition image to be DST for copy operation
        aux::TransitionImageLayout(ctx->logical, ctx->graphicsQueue, ctx->commandPool, tex_img->getImage(),
                                   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // Copy image data
        aux::CopyImageBuffer(ctx->logical, ctx->graphicsQueue, ctx->commandPool, image_staging_buffer.get(),
                             tex_img->getImage(), tex_width, tex_height);

        // Transition image to be shader readable for shader
        aux::TransitionImageLayout(ctx->logical, ctx->graphicsQueue, ctx->commandPool, tex_img->getImage(),
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        tex_img->createImageView(VK_IMAGE_ASPECT_COLOR_BIT);
        return std::make_shared<VulkanTexture>(tex_img);
    }
} // namespace ce
