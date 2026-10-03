#include "DescriptorSetLayout.hpp"
#include <stdexcept>

namespace ce {
    void DescriptorSetLayout::destroy() {
        if (handle_ != VK_NULL_HANDLE && device_ != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(device_, handle_, nullptr);
            handle_ = VK_NULL_HANDLE;
        }
    }

    void DescriptorSetLayout::create(void* extended_info, const VkDescriptorSetLayoutCreateFlags& flags) {

        // Create Desciptor Set Layout with given bindingd
        const VkDescriptorSetLayoutCreateInfo layout_create_info{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = extended_info,
            .flags = flags,
            .bindingCount = static_cast<uint32_t>(layout_binding_.size()), // Number of binding infos
            .pBindings = layout_binding_.data()                            // Array of binding infos
        };

        // Create Descriptor Set Layout
        if (vkCreateDescriptorSetLayout(device_, &layout_create_info, nullptr, &handle_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor set Layout!");
        }

        layout_binding_.clear();
        layout_binding_.shrink_to_fit();
    }
} // namespace ce
