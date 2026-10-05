#include "VBO.hpp"
#include "CmdBuffer.hpp"

namespace ce {

    void VBO::destroy() {
        count_ = 0;
        buffer_.destroy();
    }

    void VBO::create(VkQueue queue, VkCommandPool command_pool, std::vector<Vertex>* vertices, size_t size_vertex) {

        // Get size of buffer needed for vertices
        VkDeviceSize buffer_size = size_vertex * vertices->size();

        count_ = vertices->size();

        // Temporary buffer to "stage" vertex data before transfering to GPU
        Buffer staging_buffer(physical_, logical_);

        staging_buffer.create(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        staging_buffer.mapper(vertices->data());

        buffer_.init(physical_, logical_);
        // Create buffer with TRANSFER_DST_BIT to mark as recipient of transfer data (also VERTEX_BUFFER)
        // Buffer memory is to be DEVICE_LOCAL_BIT meaning memory is on the GPU and only accessible by it and not
        // CPU(host)
        buffer_.create(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Copy staging buffer to vertex buffer on GPU
        aux::copy_buffer(logical_, queue, command_pool, staging_buffer.get(), buffer_.get(), buffer_size);
    }
} // namespace ce
