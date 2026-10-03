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
        virtual ~UniformSampler() { destroy(); };

        void init(VkDevice logical) {
            logical_ = logical;
            descriptor_set_layout_.init(logical);
        }

        void destroy() {
            for (size_t i = 0; i < images_.size(); i++) {
                images_[i].reset();
            }
            images_.clear();
        }

        std::pair<size_t, size_t> allocate_descriptor_sets_with_pool(size_t tot,
                                                                     const VkDescriptorPool& descriptor_pool,
                                                                     void* variable_count_info = VK_NULL_HANDLE) {

            size_t start = descriptors_.size();
            for (size_t i = 0; i < tot; i++) {
                size_t pos = descriptors_.size();
                descriptors_.push_back(DescriptorSet{});
                descriptors_[pos].init(logical_);
                descriptors_[pos].alloc(descriptor_pool, descriptor_set_layout_.get(), variable_count_info);
            }

            return {start, tot};
        }

        DescriptorSet& get_descriptor_set(size_t index) { return descriptors_[index]; }
        DescriptorSetLayout& get_descriptor_set_layout() { return descriptor_set_layout_; }

        std::vector<std::shared_ptr<Image>>& get_images() { return images_; }

      private:
        VkDevice logical_{VK_NULL_HANDLE};
        DescriptorSetLayout descriptor_set_layout_;
        std::vector<DescriptorSet> descriptors_;
        std::vector<std::shared_ptr<Image>> images_;
    };

    class UniformBuffer {
      public:
        explicit UniformBuffer() = default;
        virtual ~UniformBuffer() { destroy(); }

        void init(VkPhysicalDevice physical, VkDevice logical, const size_t max_ubo, const size_t size_data_ubo) {

            logical_ = logical;
            // ViewProjection Buffer size
            const VkDeviceSize vp_buffer_size = size_data_ubo; // tamanho do struct com os dados

            // One uniform buffer for each image (and by extention, command buffer)
            buffers_.resize(max_ubo); // total a ser criado

            // Create Unifor buffers
            for (size_t i = 0; i < max_ubo; i++) {
                buffers_[i] = std::make_shared<Buffer>(physical, logical);
                buffers_[i]->create(vp_buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            }

            // UNIFORM VALUES DESCRIPTOR SET LAYOUT AND DESCRIPTORSETS
            descriptor_set_layout_.init(logical);
            // descriptorSets.init(logical);
        }

        void destroy() {
            for (size_t i = 0; i < buffers_.size(); i++) {
                buffers_[i].reset();
            }
            buffers_.clear();
        }

        std::pair<size_t, size_t> allocate_descriptor_sets_with_pool(size_t tot,
                                                                     const VkDescriptorPool& descriptor_pool) {

            size_t start = descriptors_.size();
            for (size_t i = 0; i < tot; i++) {
                size_t pos = descriptors_.size();
                descriptors_.push_back(DescriptorSet{});
                descriptors_[pos].init(logical_);
                descriptors_[pos].alloc(descriptor_pool, descriptor_set_layout_.get());
            }

            return {start, tot};
        }

        DescriptorSet& get_descriptor_set(size_t index) { return descriptors_[index]; }
        DescriptorSetLayout& get_descriptor_set_layout() { return descriptor_set_layout_; }

        std::vector<std::shared_ptr<Buffer>>& get_buffers() { return buffers_; }

      private:
        VkDevice logical_{VK_NULL_HANDLE};
        // DescriptorSet descriptorSets;
        DescriptorSetLayout descriptor_set_layout_;
        std::vector<DescriptorSet> descriptors_;
        std::vector<std::shared_ptr<Buffer>> buffers_;
    };

} // namespace ce
