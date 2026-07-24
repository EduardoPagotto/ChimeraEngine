#include "DescriptorSetLayout.hpp"
#include <stdexcept>

namespace ce {
    void DescriptorSetLayout::destroy() {
        if (this->handle != VK_NULL_HANDLE && this->device != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(this->device, this->handle, nullptr);
            this->handle = VK_NULL_HANDLE;
        }
    }

    void DescriptorSetLayout::create() {

        // Create Desciptor Set Layout with given bindingd
        const VkDescriptorSetLayoutCreateInfo layoutCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<uint32_t>(layoutBinding.size()), // Number of binding infos
            .pBindings = layoutBinding.data()                            // Array of binding infos
        };

        // Create Descriptor Set Layout
        if (vkCreateDescriptorSetLayout(this->device, &layoutCreateInfo, nullptr, &this->handle) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor set Layout!");
        }

        layoutBinding.clear();
        layoutBinding.shrink_to_fit();
    }
} // namespace ce
