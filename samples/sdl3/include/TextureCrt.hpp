#pragma once
#include "DescriptorPool.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "UBO.hpp"
#include "VulkanContext.hpp"
#include "VulkanTexture.hpp"

namespace ce {

    class BindlessUniformSampler {

      public:
        explicit BindlessUniformSampler(VulkanContext& context) : context(context) {
            uniform.init(context.logical);
            // globalBindlessDescriptorSet.init(context.logical);
        }

        virtual ~BindlessUniformSampler() {
            descriptorPool.destroy();
            uniform.destroy();
        }

        std::pair<size_t, size_t> create() {

            DescriptorSetLayout& bindlessLayout = uniform.getDescriptorSetLayout();

            //------------------------------------------------------------------------------------
            // --- ETAPA A: DEFINIR O LAYOUT BINDLESS ---
            //------------------------------------------------------------------------------------
            bindlessLayout.addBinding(VkDescriptorSetLayoutBinding{
                .binding = 0,
                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount = 10000, // Tamanho máximo do array (capacidade total de texturas)
                .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT});

            // Flags críticas para o comportamento Bindless
            // VkDescriptorBindingFlags bindingFlags = VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;
            const VkDescriptorBindingFlags bindingFlags =
                VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT | // Permite atualizar o set após vinculá-lo na GPU
                VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT;    // Permite ter índices vazios (sem textura alocada)

            VkDescriptorSetLayoutBindingFlagsCreateInfo extendedInfo = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
                .bindingCount = 1,
                .pBindingFlags = &bindingFlags};

            bindlessLayout.create(static_cast<void*>(&extendedInfo),
                                  VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT);

            //------------------------------------------------------------------------------------
            // --- ETAPA B: CRIAR O DESCRIPTOR POOL ---
            //------------------------------------------------------------------------------------
            this->descriptorPool.addPoolSize(
                VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 10000});

            // Ativa suporte a bindless no pool e Precisamos de apenas 1 set único global
            this->descriptorPool.create(context.logical, 1, VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT);

            //------------------------------------------------------------------------------------
            // --- ETAPA C: ALOCAR O DESCRIPTOR SET ÚNICO ---
            //------------------------------------------------------------------------------------
            uint32_t maxTextures = 10000;
            VkDescriptorSetVariableDescriptorCountAllocateInfo variableCountInfo = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
                .descriptorSetCount = 1,
                .pDescriptorCounts = &maxTextures};

            return uniform.allocateDescriptorSetsWithPool(1, this->descriptorPool.get(),
                                                          static_cast<void*>(&variableCountInfo));
        }

        void addImgsUniform(VkSampler globalSampler, std::shared_ptr<VulkanTexture> tex) {

            this->uniform.getImages().push_back(tex->get());
            //
            //  Atualiza o Descriptor Set global colocando esta nova imagem no seu respectivo índice
            VkDescriptorImageInfo imageInfo = {
                .sampler = globalSampler,                // Pode usar um sampler global ou um específico por textura
                .imageView = tex->get()->getImageView(), // imageView,
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            };

            ce::DescriptorSet& globalBindlessDescriptorSet = this->uniform.getDescriptorSet(0);
            ce::DescriptorSetWrite dsw(this->context.logical);

            dsw.add(VkWriteDescriptorSet{.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                                         .dstSet = globalBindlessDescriptorSet.get(), // O set global gigante
                                         .dstBinding = 0,                             // Binding 0 do shader
                                         .dstArrayElement = tex->getBindlessIndex(),  // Posição no array do shader
                                         .descriptorCount = 1,
                                         .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                         .pImageInfo = &imageInfo});

            // Atualiza imediatamente (Vulkan permite isso mesmo se o set estiver em uso por conta do UPDATE_AFTER_BIND)
            dsw.update();
        }

      private:
        VulkanContext& context;
        DescriptorPool descriptorPool;
        UniformSampler uniform;

    }; // namespace ce

} // namespace ce
