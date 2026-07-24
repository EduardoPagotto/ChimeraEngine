#include "VBO.hpp"
#include "CmdBuffer.hpp"

namespace ce {

    void VBO::destroy() {
        this->count = 0;
        this->buffer.destroy();
    }

    void VBO::create(VkQueue queue, VkCommandPool commandPool, std::vector<Vertex>* vertices, size_t sizeVertex) {

        // Get size of buffer needed for vertices
        VkDeviceSize bufferSize = sizeVertex * vertices->size();

        this->count = vertices->size();

        // Temporary buffer to "stage" vertex data before transfering to GPU
        Buffer stagingBuffer(physical, logical);

        stagingBuffer.create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingBuffer.mapper(vertices->data());

        this->buffer.init(physical, logical);
        // Create buffer with TRANSFER_DST_BIT to mark as recipient of transfer data (also VERTEX_BUFFER)
        // Buffer memory is to be DEVICE_LOCAL_BIT meaning memory is on the GPU and only accessible by it and not
        // CPU(host)
        this->buffer.create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Copy staging buffer to vertex buffer on GPU
        aux::CopyBuffer(this->logical, queue, commandPool, stagingBuffer.get(), this->buffer.get(), bufferSize);
    }
} // namespace ce
