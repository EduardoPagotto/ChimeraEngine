#ifndef __DESCRIPTORS_CLASS_HPP
#define __DESCRIPTORS_CLASS_HPP

#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    // -- DescriptorPool

    class DescriptorPool {
      public:
        explicit DescriptorPool(VkDevice device) : device(device) {}
        virtual ~DescriptorPool() { cleanup(); }

        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator=(const DescriptorPool&) = delete;
        // DescriptorPool(DescriptorPool&& other) noexcept;
        // DescriptorPool& operator=(DescriptorPool&& other) noexcept;
        // explicit operator VkDescriptorPool() const noexcept;

        [[nodiscard]] VkDescriptorPool& get() { return handle; }

        void addPoolSize(const VkDescriptorType& type, const uint32_t& count);
        void create(const uint32_t& maxSets);
        void cleanup() noexcept;

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkDescriptorPool handle{VK_NULL_HANDLE};
        std::vector<VkDescriptorPoolSize> poolSize;
    };

    // -- DescriptorSet

    class DescriptorSet {
      public:
        explicit DescriptorSet(VkDevice device) : device(device) {}
        virtual ~DescriptorSet() = default;

        DescriptorSet(const DescriptorSet&) = delete;
        DescriptorSet& operator=(const DescriptorSet&) = delete;
        // DescriptorSet(DescriptorSet&& other) noexcept;
        // DescriptorSet& operator=(DescriptorSet&& other) noexcept;

        std::pair<size_t, size_t> allocate(const VkDescriptorPool& descriptorPool,
                                           std::vector<VkDescriptorSetLayout>& descriptorSetLayouts);

        [[nodiscard]] std::vector<VkDescriptorSet>& get() { return this->descriptorSets; }

      private:
        VkDevice device{VK_NULL_HANDLE};
        std::vector<VkDescriptorSet> descriptorSets;
    };

    // -- DescriptorSetLayout

    class DescriptorSetLayout {
      public:
        explicit DescriptorSetLayout(VkDevice device) : device(device) {}
        virtual ~DescriptorSetLayout();

        DescriptorSetLayout(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
        // DescriptorSetLayout(DescriptorSetLayout&& other) noexcept;
        // DescriptorSetLayout& operator=(DescriptorSetLayout&& other) noexcept;

        void create();

        void addBinding(const VkDescriptorSetLayoutBinding& vpLayoutBinding) {
            this->layoutBinding.push_back(vpLayoutBinding);
        }
        [[nodiscard]] VkDescriptorSetLayout& get() { return this->handle; }

      private:
        VkDevice device{VK_NULL_HANDLE};
        VkDescriptorSetLayout handle{VK_NULL_HANDLE};
        std::vector<VkDescriptorSetLayoutBinding> layoutBinding;
    };
} // namespace ce

#endif
