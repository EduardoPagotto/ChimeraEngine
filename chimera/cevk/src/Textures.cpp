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

    Textures::Textures(ce::VulkanContext& context) : context(context) {
        //
        this->uniformSampler.init(context.logical);
        this->createDescriptorSetLayout();
        this->createDescriptorPool();
        this->createTextureSampler();
    }

    Textures::~Textures() {
        //
        vkDestroySampler(this->context.logical, this->textureSampler, nullptr);
        samplerDescriptorPool.destroy();
        uniformSampler.destroy();
    }

    int Textures::createTexture(const std::string& filename) {
        // Create Texture image and get its location in array
        int textureImageLoc = this->createTextureImage(filename);
        std::shared_ptr<Image>& image = this->uniformSampler.getImages()[textureImageLoc];

        image->createImageView(VK_IMAGE_ASPECT_COLOR_BIT);

        // Create Texture Descriptor and return location of set with texture
        return this->createTextureDescriptor(image->getImageView());
    }

    void Textures::createDescriptorSetLayout() {
        // CREATE TEXTURE SAMPLER DESCRIPTOR SET LAYOUT
        // Texture binding info
        ce::DescriptorSetLayout& samplerDSL = this->uniformSampler.getDescriptorSetLayout();
        samplerDSL.addBinding({.binding = 0,
                               .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                               .descriptorCount = 1,
                               .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                               .pImmutableSamplers = nullptr});

        samplerDSL.create();
    }

    void Textures::createDescriptorPool() {
        // CREATE SAMPLER DESCRIPTOR POOL
        this->samplerDescriptorPool.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_OBJECTS);
        this->samplerDescriptorPool.create(this->context.logical, MAX_OBJECTS,
                                           static_cast<VkDescriptorPoolCreateFlagBits>(0));
    }

    void Textures::createTextureSampler() {
        // Sampler create info
        const VkSamplerCreateInfo samplerCreateInfo{
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter = VK_FILTER_LINEAR,                   // How torender when image is magnified on screen
            .minFilter = VK_FILTER_LINEAR,                   // How to render when image is minifield on screen
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,     // Mipmap interpolation mode
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,  // texture wrap in U(x) direction wrap_s
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,  // texture wrap in V(y) direction wrap_t
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,  // texture wrap in W(z) direction wrap_r
            .mipLodBias = 0.0F,                              // Level of detail of bias for mip level
            .anisotropyEnable = VK_TRUE,                     // Enable anisotropy
            .maxAnisotropy = 16,                             // Anisotropy sample level
            .minLod = 0.0F,                                  // Minimum Level Detail ro pick mip level
            .maxLod = 0.0F,                                  // Maximum Level Detail ro pick mip level
            .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK, // Border beond texture (only works for border clamp)
            .unnormalizedCoordinates = VK_FALSE,             // Wheter coords should be normalized (between 0 and 1)
        };

        if (vkCreateSampler(context.logical, &samplerCreateInfo, nullptr, &this->textureSampler) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a Sampler");
        }
    }

    int Textures::createTextureImage(const std::string& filename) {

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
        Buffer imageStagingBuffer(context.physical, context.logical);
        imageStagingBuffer.create(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // copy image data to staging buffer
        imageStagingBuffer.mapper(surface->pixels);

        // Os pixels já estão na memória do Vulkan. Podemos destruir a superfície SDL.
        SDL_DestroySurface(surface);

        // create image to hold final texture
        std::shared_ptr<Image> texImageObj = std::make_shared<Image>(context.physical, context.logical);

        texImageObj->createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
                                 VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // COPY DATA TO IMAGE
        // Transition image to be DST for copy operation
        aux::TransitionImageLayout(this->context.logical, context.graphicsQueue, context.commandPool,
                                   texImageObj->getImage(), VK_IMAGE_LAYOUT_UNDEFINED,
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // Copy image data
        aux::CopyImageBuffer(this->context.logical, context.graphicsQueue, context.commandPool,
                             imageStagingBuffer.get(), texImageObj->getImage(), texWidth, texHeight);

        // Transition image to be shader readable for shader
        aux::TransitionImageLayout(this->context.logical, context.graphicsQueue, context.commandPool,
                                   texImageObj->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        // add texture data to vector for reference
        this->uniformSampler.getImages().push_back(texImageObj);

        return this->uniformSampler.getImages().size() - 1;
    }

    int Textures::createTextureDescriptor(VkImageView textureImage) {
        //
        auto [index, size] = this->uniformSampler.allocateDescriptorSetsWithPool(1, this->samplerDescriptorPool.get());

        // Texture Image info
        const VkDescriptorImageInfo imageInfo{
            .sampler = this->textureSampler,                        // Image layout when in use
            .imageView = textureImage,                              // Sampler to use for set
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL // Image to bind to set
        };

        ce::DescriptorSet& samplerDS = this->uniformSampler.getDescriptorSet();

        // Descriptor Write info
        const VkWriteDescriptorSet descriptorWrite{.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                                                   .dstSet = samplerDS.get(index),
                                                   .dstBinding = 0,
                                                   .dstArrayElement = 0,
                                                   .descriptorCount = 1,
                                                   .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                   .pImageInfo = &imageInfo};

        samplerDS.addWrite(descriptorWrite);
        // Update new descriptor set
        samplerDS.update();
        samplerDS.clearWrite();

        return samplerDS.getSize() - 1;
    }
} // namespace ce
