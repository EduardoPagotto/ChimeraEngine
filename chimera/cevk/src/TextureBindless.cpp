#include "TextureBindless.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"

namespace ce {

    TextureBindless::TextureBindless(std::shared_ptr<VulkanContext> ctx) : ctx_(ctx) {
        //
        uniform_.init(ctx->logical);
        //------------------------------------------------------------------------------------
        // 1. CREATE DESCRIPTOR SET LAYOUT BINDLESS
        //------------------------------------------------------------------------------------
        DescriptorSetLayout& layout = uniform_.get_descriptor_set_layout();
        layout.add_binding(VkDescriptorSetLayoutBinding{
            .binding = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 10000, // Tamanho máximo do array (capacidade total de texturas)
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT});

        // Flags críticas para o comportamento Bindless
        const VkDescriptorBindingFlags binding_flags =
            VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT | // Permite atualizar o set após vinculá-lo na GPU
            VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT;    // Permite ter índices vazios (sem textura alocada)

        VkDescriptorSetLayoutBindingFlagsCreateInfo extended_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
            .bindingCount = 1,
            .pBindingFlags = &binding_flags};

        layout.create(static_cast<void*>(&extended_info), VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT);

        //------------------------------------------------------------------------------------
        // 2. CREATE DESCRIPTOR POOL
        //------------------------------------------------------------------------------------
        descriptor_pool_.add_pool_size(
            VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 10000});

        // Ativa suporte a bindless no pool e Precisamos de apenas 1 set único global
        descriptor_pool_.create(ctx->logical, 1, VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT);

        //------------------------------------------------------------------------------------
        // 3. ALOCAR O DESCRIPTOR SET ÚNICO ---
        //------------------------------------------------------------------------------------
        uint32_t max_textures = 10000;
        VkDescriptorSetVariableDescriptorCountAllocateInfo variable_count_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
            .descriptorSetCount = 1,
            .pDescriptorCounts = &max_textures};

        uniform_.allocate_descriptor_sets_with_pool(1, descriptor_pool_.get(),
                                                    static_cast<void*>(&variable_count_info));

        //------------------------------------------------------------------------------------
        //  4. CREATE TEXTURE SAMPLER
        //------------------------------------------------------------------------------------
        tex_sampler_.init(ctx->logical);
    }

    TextureBindless::~TextureBindless() {
        tex_sampler_.destroy();
        descriptor_pool_.destroy();
        uniform_.destroy();
    }

    uint32_t TextureBindless::alloc_texture(std::shared_ptr<VulkanTexture> tex) {

        //   Atualiza o Descriptor Set global colocando esta nova imagem no seu respectivo índice
        VkDescriptorImageInfo image_info = {
            .sampler = tex_sampler_.get(),           // Pode usar um sampler global ou um específico por textura
            .imageView = tex->get()->getImageView(), // imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        DescriptorSet& descriptor_set = uniform_.get_descriptor_set(0);

        DescriptorSetWrite dsw(ctx_->logical);
        // Descriptor Write info
        dsw.add(VkWriteDescriptorSet{.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                                     .dstSet = descriptor_set.get(),               // O set global gigante
                                     .dstBinding = 0,                              // Binding 0 do shader
                                     .dstArrayElement = tex->get_bindless_index(), // Posição no array do shader
                                     .descriptorCount = 1,
                                     .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                     .pImageInfo = &image_info});

        // Atualiza imediatamente (Vulkan permite isso mesmo se o set estiver em uso por conta do UPDATE_AFTER_BIND)
        dsw.update();
        return tex->get_bindless_index();
    }

} // namespace ce
