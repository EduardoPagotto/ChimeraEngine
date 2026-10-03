#include "Textures.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "UBO.hpp"
#include "cevk.hpp"
#include <SDL3_image/SDL_image.h>

namespace ce {

    Textures::Textures(std::shared_ptr<VulkanContext> ctx) : ctx_(ctx) {
        //
        uniform_.init(ctx->logical);
        //------------------------------------------------------------------------------------
        // 1. CREATE DESCRIPTOR SET LAYOUT (SAMPLER), Texture binding info
        //------------------------------------------------------------------------------------
        DescriptorSetLayout& layout = uniform_.get_descriptor_set_layout();
        layout.add_binding(VkDescriptorSetLayoutBinding{.binding = 0,
                                                        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                        .descriptorCount = 1,
                                                        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                                                        .pImmutableSamplers = nullptr});

        layout.create();

        //------------------------------------------------------------------------------------
        // 2. CREATE DESCRIPTOR POOL
        //------------------------------------------------------------------------------------
        descriptor_pool_.add_pool_size(
            VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = max_objects});

        descriptor_pool_.create(ctx_->logical, max_objects, static_cast<VkDescriptorPoolCreateFlagBits>(0));

        //------------------------------------------------------------------------------------
        //  3. CREATE TEXTURE SAMPLER
        //------------------------------------------------------------------------------------
        tex_sampler_.init(ctx->logical);
    }

    Textures::~Textures() {
        tex_sampler_.destroy();
        descriptor_pool_.destroy();
        uniform_.destroy();
    }

    uint32_t Textures::alloc_texture(std::shared_ptr<VulkanTexture> vulkan_tex) {

        std::shared_ptr<Image> tex_image_obj = vulkan_tex->get();
        uniform_.get_images().push_back(tex_image_obj);

        //
        auto [index, size] = uniform_.allocate_descriptor_sets_with_pool(1, descriptor_pool_.get());

        // Texture Image info
        const VkDescriptorImageInfo image_info{
            .sampler = tex_sampler_.get(),                          // Image layout when in use
            .imageView = tex_image_obj->getImageView(),             // Sampler to use for set
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL // Image to bind to set
        };

        DescriptorSet& descriptor_set = uniform_.get_descriptor_set(index);

        DescriptorSetWrite dsw(ctx_->logical);
        // Descriptor Write info
        dsw.add(VkWriteDescriptorSet{.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                                     .dstSet = descriptor_set.get(),
                                     .dstBinding = 0,
                                     .dstArrayElement = 0,
                                     .descriptorCount = 1,
                                     .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                     .pImageInfo = &image_info});

        dsw.update();
        return index;
    }
} // namespace ce
