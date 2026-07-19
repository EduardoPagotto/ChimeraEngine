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
            this->descriptorSetLayout = std::make_shared<DescriptorSetLayout>(this->logical);
            this->descriptorSets = std::make_shared<DescriptorSet>(this->logical);
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
            this->descriptorSetLayout = std::make_shared<DescriptorSetLayout>(this->logical);
            this->descriptorSets = std::make_shared<DescriptorSet>(this->logical);
        }

        virtual ~UBO() {

            // for (size_t i = 0; i < sizeDataUBO; i++) {
            //     this->ubo[i].reset();
            // }

            this->descriptorSets.reset(); // FIXME: acima da erro
            this->descriptorSetLayout.reset();
        }

        void addDescriptorSetLayoutBinding(const VkDescriptorSetLayoutBinding& vpLayoutBinding) {
            this->descriptorSetLayout->addBinding(vpLayoutBinding);
        }

        void createDescriptorSetLayout() { this->descriptorSetLayout->create(); }

        void addWriteDescriptorSet(const VkWriteDescriptorSet& vpSetWrite) { this->setWrites.push_back(vpSetWrite); }
        void clearWriteDescriptorSet() { this->setWrites.clear(); }

        std::pair<size_t, size_t> allocateDescriptorSets(size_t tot, const VkDescriptorPool& descriptorPool) {
            std::vector<VkDescriptorSetLayout> setLayouts(tot, this->descriptorSetLayout->get());
            return this->descriptorSets->allocate(descriptorPool, setLayouts);
        }

        void updateDescriptorSets() {
            // Update the descripto sets with new buffer/binding info
            vkUpdateDescriptorSets(logical, static_cast<uint32_t>(this->setWrites.size()), this->setWrites.data(), 0,
                                   nullptr);
        }

        [[nodiscard]] size_t size() const noexcept { return ubo.size(); }
        [[nodiscard]] std::vector<std::shared_ptr<T>>& getUBO() { return ubo; }
        [[nodiscard]] VkDescriptorSetLayout& getDescriptorSetLayout() const { return descriptorSetLayout->get(); }
        [[nodiscard]] std::vector<VkDescriptorSet>& getDescriptorSets() const { return descriptorSets->get(); }

      private:
        VkDevice logical;
        std::shared_ptr<DescriptorSet> descriptorSets;
        std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
        Container<std::shared_ptr<T>, std::allocator<std::shared_ptr<T>>> ubo;

        std::vector<VkWriteDescriptorSet> setWrites;
    };

} // namespace ce
