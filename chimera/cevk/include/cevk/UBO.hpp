#pragma once

#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include <memory>

namespace ce {

    template <typename T, template <typename, typename> class Container = std::vector>
    class UBO {
      public:
        explicit UBO(VkDevice logical) : logical(logical) {
            // UNIFORM VALUES DESCRIPTOR SET LAYOUT AND DESCRIPTORSETS
            this->descriptorSetLayout.init(this->logical);
            this->descriptorSets.init(this->logical);
        }
        explicit UBO(VkPhysicalDevice physical, VkDevice logical, const size_t maxUBO, const size_t sizeDataUBO)
            : logical(logical) {

            // ViewProjection Buffer size
            const VkDeviceSize vpBufferSize = sizeDataUBO; // tamanho do struct com os dados

            // One uniform buffer for each image (and by extention, command buffer)
            this->ubo.resize(maxUBO); // total a ser criado

            // Create Unifor buffers
            for (size_t i = 0; i < maxUBO; i++) {
                this->ubo[i] = std::make_shared<T>(physical, logical);
                this->ubo[i]->create(vpBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            }

            // UNIFORM VALUES DESCRIPTOR SET LAYOUT AND DESCRIPTORSETS
            this->descriptorSetLayout.init(this->logical);
            this->descriptorSets.init(this->logical);
        }

        virtual ~UBO() {

            // for (size_t i = 0; i < sizeDataUBO; i++) {
            //     this->ubo[i].reset();
            // }

            // this->descriptorSets.reset(); // FIXME: acima da erro
            // this->descriptorSetLayout.reset();
        }

        std::pair<size_t, size_t> allocateDescriptorSetsWithPool(size_t tot, const VkDescriptorPool& descriptorPool) {
            std::vector<VkDescriptorSetLayout> setLayouts(tot, this->descriptorSetLayout.get());
            return this->descriptorSets.allocate(descriptorPool, setLayouts);
        }

        size_t size() const noexcept { return ubo.size(); }
        std::vector<std::shared_ptr<T>>& getUBO() { return ubo; }

        DescriptorSetLayout& getDescriptorSetLayout() { return descriptorSetLayout; }
        DescriptorSet& getDescriptorSet() { return this->descriptorSets; }

      private:
        VkDevice logical;
        DescriptorSet descriptorSets;
        DescriptorSetLayout descriptorSetLayout;

        Container<std::shared_ptr<T>, std::allocator<std::shared_ptr<T>>> ubo;
    };

} // namespace ce
