#include "Textures.hpp"
#include "Buffers.hpp"
#include "CmdBuffer.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "UBO.hpp"
#include "cevk.hpp"
#include <SDL3_image/SDL_image.h>
#include <format>

namespace ce {

    Textures::Textures(std::shared_ptr<VulkanContext> ctx) : ctx(ctx) {
        //
        this->uniformSampler.init(ctx->logical);
        //------------------------------------------------------------------------------------
        // CREATE DESCRIPTOR SET LAYOUT (SAMPLER), Texture binding info
        //------------------------------------------------------------------------------------
        ce::DescriptorSetLayout& samplerDSL = this->uniformSampler.getDescriptorSetLayout();
        samplerDSL.addBinding(VkDescriptorSetLayoutBinding{.binding = 0,
                                                           .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                           .descriptorCount = 1,
                                                           .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                                                           .pImmutableSamplers = nullptr});

        samplerDSL.create();

        //------------------------------------------------------------------------------------
        // CREATE DESCRIPTOR POOL (SAMPLER)
        //------------------------------------------------------------------------------------
        this->samplerDescriptorPool.addPoolSize(
            VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = MAX_OBJECTS});

        this->samplerDescriptorPool.create(this->ctx->logical, MAX_OBJECTS,
                                           static_cast<VkDescriptorPoolCreateFlagBits>(0));

        //------------------------------------------------------------------------------------
        //  CREATE TEXTURE SAMPLER
        //------------------------------------------------------------------------------------
        this->texSampler.init(ctx->logical);
    }

    Textures::~Textures() {
        texSampler.destroy();
        samplerDescriptorPool.destroy();
        uniformSampler.destroy();
    }

    int Textures::createTexture(const std::string& filename) {
        // Create Texture image
        std::shared_ptr<Image> texImageObj = this->createTextureImage(filename);
        this->uniformSampler.getImages().push_back(texImageObj);

        // Create Texture Descriptor and return location of set with texture
        return this->createTextureDescriptor(texImageObj->getImageView());
    }

    std::shared_ptr<Image> Textures::createTextureImage(const std::string& filename) {

        std::string fileLoc = "./assets/textures/" + filename;
        SDL_Surface* loadedSurface = IMG_Load(fileLoc.c_str());
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
        std::shared_ptr<Image> texImageObj = std::make_shared<Image>(ctx->physical, ctx->logical);

        texImageObj->createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
                                 VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // COPY DATA TO IMAGE
        // Transition image to be DST for copy operation
        aux::TransitionImageLayout(this->ctx->logical, ctx->graphicsQueue, ctx->commandPool, texImageObj->getImage(),
                                   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // Copy image data
        aux::CopyImageBuffer(this->ctx->logical, ctx->graphicsQueue, ctx->commandPool, imageStagingBuffer.get(),
                             texImageObj->getImage(), texWidth, texHeight);

        // Transition image to be shader readable for shader
        aux::TransitionImageLayout(this->ctx->logical, ctx->graphicsQueue, ctx->commandPool, texImageObj->getImage(),
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        texImageObj->createImageView(VK_IMAGE_ASPECT_COLOR_BIT);

        return texImageObj;
    }

    int Textures::createTextureDescriptor(VkImageView textureImage) {
        //
        auto [index, size] = this->uniformSampler.allocateDescriptorSetsWithPool(1, this->samplerDescriptorPool.get());

        // Texture Image info
        const VkDescriptorImageInfo imageInfo{
            .sampler = this->texSampler.get(),                      // Image layout when in use
            .imageView = textureImage,                              // Sampler to use for set
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL // Image to bind to set
        };

        ce::DescriptorSet& samplerDS = this->uniformSampler.getDescriptorSet(index);

        ce::DescriptorSetWrite dsw(this->ctx->logical);
        // Descriptor Write info
        dsw.add(VkWriteDescriptorSet{.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                                     .dstSet = samplerDS.get(),
                                     .dstBinding = 0,
                                     .dstArrayElement = 0,
                                     .descriptorCount = 1,
                                     .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                     .pImageInfo = &imageInfo});

        dsw.update();

        // return samplerDS.getSize() - 1;
        return index;
    }
} // namespace ce
