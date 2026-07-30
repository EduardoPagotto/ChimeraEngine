#pragma once

#include "Buffers.hpp"
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"
#include "Image.hpp"
#include <memory>

namespace ce {

    class UniformSampler {
      public:
        explicit UniformSampler() = default;
        virtual ~UniformSampler() { this->destroy(); };

        void init(VkDevice logical) {
            this->logical = logical;
            this->descriptorSetLayout.init(logical);
        }

        void destroy() {
            for (size_t i = 0; i < images.size(); i++) {
                this->images[i].reset();
            }
            this->images.clear();
        }

        std::pair<size_t, size_t> allocateDescriptorSetsWithPool(size_t tot, const VkDescriptorPool& descriptorPool) {

            size_t start = descriptors.size();
            for (size_t i = 0; i < tot; i++) {
                size_t pos = descriptors.size();
                descriptors.push_back(DescriptorSet{});
                descriptors[pos].init(this->logical);
                descriptors[pos].alloc(descriptorPool, this->descriptorSetLayout.get());
            }

            return {start, tot};
        }

        DescriptorSet& getDescriptorSet(size_t index) { return this->descriptors[index]; }
        DescriptorSetLayout& getDescriptorSetLayout() { return this->descriptorSetLayout; }

        std::vector<std::shared_ptr<Image>>& getImages() { return images; }

      private:
        VkDevice logical{VK_NULL_HANDLE};
        DescriptorSetLayout descriptorSetLayout;
        std::vector<DescriptorSet> descriptors;
        std::vector<std::shared_ptr<Image>> images;
    };

    class UniformBuffer {
      public:
        explicit UniformBuffer() = default;
        virtual ~UniformBuffer() { this->destroy(); }

        void init(VkPhysicalDevice physical, VkDevice logical, const size_t maxUBO, const size_t sizeDataUBO) {

            this->logical = logical;
            // ViewProjection Buffer size
            const VkDeviceSize vpBufferSize = sizeDataUBO; // tamanho do struct com os dados

            // One uniform buffer for each image (and by extention, command buffer)
            this->buffers.resize(maxUBO); // total a ser criado

            // Create Unifor buffers
            for (size_t i = 0; i < maxUBO; i++) {
                this->buffers[i] = std::make_shared<Buffer>(physical, logical);
                this->buffers[i]->create(vpBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            }

            // UNIFORM VALUES DESCRIPTOR SET LAYOUT AND DESCRIPTORSETS
            descriptorSetLayout.init(logical);
            // descriptorSets.init(logical);
        }

        void destroy() {
            for (size_t i = 0; i < buffers.size(); i++) {
                this->buffers[i].reset();
            }
            this->buffers.clear();
        }

        std::pair<size_t, size_t> allocateDescriptorSetsWithPool(size_t tot, const VkDescriptorPool& descriptorPool) {

            size_t start = descriptors.size();
            for (size_t i = 0; i < tot; i++) {
                size_t pos = descriptors.size();
                descriptors.push_back(DescriptorSet{});
                descriptors[pos].init(this->logical);
                descriptors[pos].alloc(descriptorPool, this->descriptorSetLayout.get());
            }

            return {start, tot};
        }

        DescriptorSet& getDescriptorSet(size_t index) { return this->descriptors[index]; }
        DescriptorSetLayout& getDescriptorSetLayout() { return this->descriptorSetLayout; }

        std::vector<std::shared_ptr<Buffer>>& getBuffers() { return buffers; }

      private:
        VkDevice logical{VK_NULL_HANDLE};
        // DescriptorSet descriptorSets;
        DescriptorSetLayout descriptorSetLayout;
        std::vector<DescriptorSet> descriptors;
        std::vector<std::shared_ptr<Buffer>> buffers;
    };

} // namespace ce
