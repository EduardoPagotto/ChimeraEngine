#include "CmdPool.hpp"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace ce {
    //
    CmdPool::CmdPool(VkDevice logical, uint32_t queueGraphicsFamilyIndex) {
        this->init(logical, queueGraphicsFamilyIndex);
    }

    CmdPool::~CmdPool() { this->destroy(); }

    void CmdPool::init(VkDevice logical, uint32_t queueGraphicsFamilyIndex) {

        this->logical = logical;

        const VkCommandPoolCreateInfo poolInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex =
                queueGraphicsFamilyIndex // Queue Family type that buffers from this command pool will use
        };

        // Create a Graphics Queue Family Command Pool
        if (vkCreateCommandPool(logical, &poolInfo, nullptr, &this->commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Command Pool");
        }
    }
    void CmdPool::destroy() {
        if (this->commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(this->logical, this->commandPool, nullptr);
            this->commandPool = VK_NULL_HANDLE;
        }
    }

    void CmdPool::cleanup() {
        //
        if (vkResetCommandPool(this->logical, this->commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to Reset Command Pool");
        }
    }
} // namespace ce
