#include "IBO.hpp"
#include "CmdBuffer.hpp"

namespace ce {

    IBO::IBO(VkPhysicalDevice physical, VkDevice logical) : physical(physical), logical(logical) {}

    IBO::~IBO() { this->destroy(); }

    void IBO::destroy() {
        this->count = 0;
        this->buffer.destroy();
    }

    void IBO::create(VkQueue queue, VkCommandPool commandBuffer, std::vector<uint32_t>* indices) {

        // Get size of buffer needed for indices
        VkDeviceSize bufferSize = sizeof(uint32_t) * indices->size();

        this->count = indices->size();

        // Temporary buffer to "stage" index data before transfering to GPU
        Buffer stagingBuffer(physical, logical);
        stagingBuffer.create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // MAP MEMORY TO INDEX BUFFER
        stagingBuffer.mapper(indices->data());

        // Create buffer for index data on GPU aceess only area
        this->buffer.init(physical, logical);
        this->buffer.create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Copy from staging buffer to GPU access buffer
        aux::CopyBuffer(this->logical, queue, commandBuffer, stagingBuffer.get(), this->buffer.get(), bufferSize);
    }

} // namespace ce
