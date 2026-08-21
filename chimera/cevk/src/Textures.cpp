#include "Textures.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "UBO.hpp"
#include "cevk.hpp"
#include <SDL3_image/SDL_image.h>

namespace ce {

    Textures::Textures(std::shared_ptr<VulkanContext> ctx) : ctx(ctx) {
        //
        this->uniform.init(ctx->logical);
        //------------------------------------------------------------------------------------
        // 1. CREATE DESCRIPTOR SET LAYOUT (SAMPLER), Texture binding info
        //------------------------------------------------------------------------------------
        DescriptorSetLayout& layout = this->uniform.getDescriptorSetLayout();
        layout.addBinding(VkDescriptorSetLayoutBinding{.binding = 0,
                                                       .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                       .descriptorCount = 1,
                                                       .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                                                       .pImmutableSamplers = nullptr});

        layout.create();

        //------------------------------------------------------------------------------------
        // 2. CREATE DESCRIPTOR POOL
        //------------------------------------------------------------------------------------
        this->descriptorPool.addPoolSize(
            VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = MAX_OBJECTS});

        this->descriptorPool.create(this->ctx->logical, MAX_OBJECTS, static_cast<VkDescriptorPoolCreateFlagBits>(0));

        //------------------------------------------------------------------------------------
        //  3. CREATE TEXTURE SAMPLER
        //------------------------------------------------------------------------------------
        this->texSampler.init(ctx->logical);
    }

    Textures::~Textures() {
        texSampler.destroy();
        descriptorPool.destroy();
        uniform.destroy();
    }

    int Textures::allocTexture(std::shared_ptr<VulkanTexture> vulkanTex) {

        std::shared_ptr<Image> texImageObj = vulkanTex->get();
        this->uniform.getImages().push_back(texImageObj);

        //
        auto [index, size] = this->uniform.allocateDescriptorSetsWithPool(1, this->descriptorPool.get());

        // Texture Image info
        const VkDescriptorImageInfo imageInfo{
            .sampler = this->texSampler.get(),                      // Image layout when in use
            .imageView = texImageObj->getImageView(),               // Sampler to use for set
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL // Image to bind to set
        };

        DescriptorSet& descriptorSet = this->uniform.getDescriptorSet(index);

        DescriptorSetWrite dsw(this->ctx->logical);
        // Descriptor Write info
        dsw.add(VkWriteDescriptorSet{.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                                     .dstSet = descriptorSet.get(),
                                     .dstBinding = 0,
                                     .dstArrayElement = 0,
                                     .descriptorCount = 1,
                                     .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                     .pImageInfo = &imageInfo});

        dsw.update();
        return index;
    }
} // namespace ce
