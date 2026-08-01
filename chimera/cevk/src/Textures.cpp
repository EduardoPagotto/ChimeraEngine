#include "Textures.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "UBO.hpp"
#include "cevk.hpp"
#include <SDL3_image/SDL_image.h>

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
        // CREATE DESCRIPTOR POOL
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

    int Textures::allocTexture(std::shared_ptr<VulkanTexture> vulkanTex) {

        std::shared_ptr<Image> texImageObj = vulkanTex->get();
        this->uniformSampler.getImages().push_back(texImageObj);

        //
        auto [index, size] = this->uniformSampler.allocateDescriptorSetsWithPool(1, this->samplerDescriptorPool.get());

        // Texture Image info
        const VkDescriptorImageInfo imageInfo{
            .sampler = this->texSampler.get(),                      // Image layout when in use
            .imageView = texImageObj->getImageView(),               // Sampler to use for set
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
