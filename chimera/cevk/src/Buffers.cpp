#include "Buffers.hpp"
#include "DevVK.hpp"
#include <array>
#include <cstring>
#include <stdexcept>

namespace ce {

#pragma region BufferObject

    BufferObject::~BufferObject() { this->destroy(); }

    void BufferObject::create(const VkDeviceSize& bufferSize, const VkBufferUsageFlags& bufferUsage,
                              const VkMemoryPropertyFlags& bufferProperties) {

        // information to create a buffer (dosen't include assigning memory)
        const VkBufferCreateInfo bufferInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = bufferSize,                       // Size of buffer (size of 1 vertex * number of vertices)
            .usage = bufferUsage,                     // Multiple types of buffer possible
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE, // Similar to Swap Chain images, can share vertex buffers
        };

        this->bufferSize = bufferSize;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &this->buffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create a Buffer!");
        }

        // GET BUFFER MEMORY REQUIREMENTS
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, this->buffer, &memRequirements);

        // ALLOCATE MEMORY TO BUFFER
        // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : CPU can interact with memory
        // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : Allows placement of data straight into buffer mapping (otherwise would
        // have to specify manually)
        const VkMemoryAllocateInfo memoryAllocInfo{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memRequirements.size,
            .memoryTypeIndex =
                aux::FindMemoryTypeIndex(this->physical, memRequirements.memoryTypeBits, bufferProperties)};

        // Allocate memory to VkDebviceMemory
        if (vkAllocateMemory(this->device, &memoryAllocInfo, nullptr, &this->memory) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate Vertex Buffer Memory!!");
        }

        // Allocate memory to given vertex buffer
        vkBindBufferMemory(this->device, this->buffer, this->memory, 0);
    }

    void BufferObject::mapper(void* src) {
        // MAP MEMORY TO BUFFER
        // 1. Get and create pointer to point in normal memory
        // 2. "Map" the vertex buffer memory to that point
        // 3. Copy memory from vertices vector to the point
        // 4. Unmap the vertex buffer memory
        void* mappedData;
        vkMapMemory(this->device, this->memory, 0, this->bufferSize, 0, &mappedData);
        std::memcpy(mappedData, static_cast<const void*>(src), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device, memory);
    }

    void BufferObject::destroy() {
        if (device == VK_NULL_HANDLE) {
            return;
        }

        // Se a memória estava mapeada, desmapeia primeiro
        // if ((mappedData != nullptr) && (memory != VK_NULL_HANDLE)) {
        //     vkUnmapMemory(device, memory);
        //     mappedData = nullptr;
        // }

        // Deleta o buffer se ele existir
        if (buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(device, buffer, nullptr);
            buffer = VK_NULL_HANDLE;
        }

        // Libera a memória alocada por último
        if (memory != VK_NULL_HANDLE) {
            vkFreeMemory(device, memory, nullptr);
            memory = VK_NULL_HANDLE;
        }

        device = VK_NULL_HANDLE;
    }

#pragma endregion

#pragma region VBO

    VBO::VBO(VkPhysicalDevice physical, VkDevice logical) : physical(physical), logical(logical) {}

    VBO::~VBO() { this->destroy(); }

    void VBO::destroy() {
        this->count = 0;
        this->vbo.reset();
    }

    void VBO::create(VkQueue queue, VkCommandPool commandPool, std::vector<Vertex>* vertices, size_t sizeVertex) {

        // Get size of buffer needed for vertices
        VkDeviceSize bufferSize = sizeVertex * vertices->size();

        this->count = vertices->size();

        // Temporary buffer to "stage" vertex data before transfering to GPU
        BufferObject stagingBuffer(physical, logical);

        stagingBuffer.create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingBuffer.mapper(vertices->data());

        this->vbo = std::make_shared<BufferObject>(physical, logical);
        // Create buffer with TRANSFER_DST_BIT to mark as recipient of transfer data (also VERTEX_BUFFER)
        // Buffer memory is to be DEVICE_LOCAL_BIT meaning memory is on the GPU and only accessible by it and not
        // CPU(host)
        this->vbo->create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Copy staging buffer to vertex buffer on GPU
        aux::CopyBuffer(this->logical, queue, commandPool, stagingBuffer.getBuffer(), this->vbo->getBuffer(),
                        bufferSize);
    }

#pragma endregion

#pragma region ImageObject

    ImageObject::~ImageObject() { this->destroy(); }

    void ImageObject::createImage(uint32_t with, uint32_t height, VkFormat format, VkImageTiling tiling,
                                  VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags) {
        // CREATE IMAGE
        // Image Create Info
        const VkImageCreateInfo imageCreateInfo{
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType = VK_IMAGE_TYPE_2D,              // Type of image (1D, 2D or 3D)
            .format = format,                           // Format type of image
            .extent = {.width = with,                   // Width of image extent
                       .height = height,                // Height of image extent
                       .depth = 1},                     // Depth of image (just 1, no 3D aspect)
            .mipLevels = 1,                             // Number of mipmaps levels
            .arrayLayers = 1,                           // Number of levels in image array
            .samples = VK_SAMPLE_COUNT_1_BIT,           // Number of samples for multi-sampling
            .tiling = tiling,                           // How image data shoud be "tiled" (arranged for optima reading)
            .usage = useFlags,                          // Bit flags defined what image will be usage for
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,   // Whether image cam be shared between queues
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED, // Layout of image data on creation
        };

        if (vkCreateImage(this->device, &imageCreateInfo, nullptr, &this->image) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create an image!");
        }

        this->format = format;
        // CREATE MEMORY FOR IMAGE

        // Get Memory requirement for a type of image
        VkMemoryRequirements memoryRequirements;
        vkGetImageMemoryRequirements(this->device, this->image, &memoryRequirements);

        // Allocate memory using image requeirement and user define properties
        const VkMemoryAllocateInfo memoryAllocInfo{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = aux::FindMemoryTypeIndex(this->physical, memoryRequirements.memoryTypeBits, propFlags)};

        if (vkAllocateMemory(this->device, &memoryAllocInfo, nullptr, &this->imageMemory) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Allocate Memory for Image");
        }

        // Connect memory to image
        vkBindImageMemory(this->device, this->image, this->imageMemory, 0);

        this->isImported = false;
    }

    void ImageObject::createImageViewImportedImage(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
        this->image = image;
        this->format = format;
        this->isImported = true;
        this->createImageView(aspectFlags);
    }

    void ImageObject::createImageView(VkImageAspectFlags aspectFlags) {
        //
        const VkImageViewCreateInfo viewCreateInfo{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,  //
            .image = this->image,                               // Image to create view for
            .viewType = VK_IMAGE_VIEW_TYPE_2D,                  // Type of image (1D, 2D, 3D, Cube, etc)
            .format = this->format,                             // Format of image data
            .components = {.r = VK_COMPONENT_SWIZZLE_IDENTITY,  // Allows remapping of rgba component to other values
                           .g = VK_COMPONENT_SWIZZLE_IDENTITY,  //
                           .b = VK_COMPONENT_SWIZZLE_IDENTITY,  //
                           .a = VK_COMPONENT_SWIZZLE_IDENTITY}, //
            .subresourceRange = {
                // Subresources allow the view to view only a part of a image
                .aspectMask = aspectFlags, // which aspect of image to view (e.g. COLOR_BIT for view color)
                .baseMipLevel = 0,         // Start mipmap level to start from
                .levelCount = 1,           // Number of mipmap levels to view
                .baseArrayLayer = 0,       // Start array level to view from
                .layerCount = 1            // Numbers of array levels to view
            }};

        // Create image view and return it
        if (vkCreateImageView(this->device, &viewCreateInfo, nullptr, &this->imageView) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create an Image View!");
        }
    }

    void ImageObject::destroy() {
        if (device == VK_NULL_HANDLE) {
            return;
        }

        if (this->imageView != VK_NULL_HANDLE) {
            vkDestroyImageView(this->device, this->imageView, nullptr);
            this->imageView = VK_NULL_HANDLE;
        }

        if ((this->image != VK_NULL_HANDLE) && (!this->isImported)) {
            vkDestroyImage(this->device, this->image, nullptr);
            this->image = {VK_NULL_HANDLE};
        }

        if (this->imageMemory != VK_NULL_HANDLE) {
            vkFreeMemory(this->device, this->imageMemory, nullptr);
            this->imageMemory = VK_NULL_HANDLE;
        }

        device = VK_NULL_HANDLE;
    }

#pragma endregion

#pragma region CommandBuffer

    CommandBuffer::CommandBuffer(VkDevice device, VkCommandPool commandPool, size_t count)
        : device(device), commandPool(commandPool) {

        this->commandBuffers.resize(count);

        const VkCommandBufferAllocateInfo cbAllocInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = commandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, // VK_COMMAND_BUFFER_LEVEL_PRIMARY : Buffer you submit directly
                                                      // to queue. Can't be called by other buffers.
                                                      // VK_COMMAND_BUFFER_LEVEL_SECUNDARY : Buffer can't be called
                                                      // directly. cam be called from other buffe via
                                                      // "VkCmdExecuteCommand" when recording commands in primary buf
            .commandBufferCount = static_cast<uint32_t>(commandBuffers.size())};

        // Allocate command buffers and place handles in array of buffers
        if (vkAllocateCommandBuffers(device, &cbAllocInfo, this->commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Allocate Command buffers!");
        }
    }

    CommandBuffer::~CommandBuffer() {
        // Free temporary command buffer back to pool
        vkFreeCommandBuffers(this->device, this->commandPool, static_cast<uint32_t>(this->commandBuffers.size()),
                             this->commandBuffers.data());
    }

    void CommandBuffer::clean(size_t index) {
        if (vkResetCommandBuffer(this->commandBuffers[index], VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to reset a Command Buffer!");
        }
    }

    void CommandBuffer::cleanAll() {
        for (auto& commandBuffer : this->commandBuffers) {
            if (vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS) {
                throw std::runtime_error("Failed to reset a Command Buffer!");
            }
        }
    }

    void CommandBuffer::begin(size_t index, VkCommandBufferUsageFlagBits flag) {

        // Information to begin the command buffer record
        const VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = flag // We're only using the command buffer once, so set up for one time submit
        };

        // Begin recording transfer commands
        if (vkBeginCommandBuffer(commandBuffers[index], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin a Command Buffer!");
        }
    }

    void CommandBuffer::end(size_t index) {
        // End commands
        if (vkEndCommandBuffer(this->commandBuffers[index]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to end a Command Buffer!");
        }
    }

    namespace aux {

        void SubmitQueue(VkQueue queue, VkCommandBuffer commandBuffer) {
            // Queue submission information
            const VkSubmitInfo submitInfo{
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO, //
                .commandBufferCount = 1,                //
                .pCommandBuffers = &commandBuffer       //
            };

            // Submit transfer command to transfer queue and wait until it finishes
            vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(queue);
        }

        void CopyBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, VkBuffer srcBuffer,
                        VkBuffer dstBuffer, VkDeviceSize bufferSize) {

            CommandBuffer transferComandBuffer(device, transferCommandPool, 1);
            transferComandBuffer.begin(0, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

            // Region of data to copy from and to
            const VkBufferCopy bufferCopyRegion{.srcOffset = 0, .dstOffset = 0, .size = bufferSize};

            // Command to copy src buffer to dst buffer
            vkCmdCopyBuffer(transferComandBuffer.getBuffers()[0], srcBuffer, dstBuffer, 1, &bufferCopyRegion);

            transferComandBuffer.end(0);
            SubmitQueue(transferQueue, transferComandBuffer.getBuffers()[0]);
        }

        void CopyImageBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool,
                             VkBuffer srcBuffer, VkImage image, uint32_t width, uint32_t height) {
            // Create Buffer
            CommandBuffer transferComandBuffer(device, transferCommandPool, 1);
            transferComandBuffer.begin(0, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

            const VkBufferImageCopy imageRegion{
                .bufferOffset = 0,      // Offset into data
                .bufferRowLength = 0,   // Row leght of data to calculate data spacing
                .bufferImageHeight = 0, // Image height to calculate data spacing
                .imageSubresource =
                    VkImageSubresourceLayers{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, // Which aspect of image to copy
                                             .mipLevel = 0,                           // Mipmap level to copy
                                             .baseArrayLayer = 0,                     // Starting array layer (if array)
                                             .layerCount = 1}, // Number of layers to copy starting ar baseArray
                .imageOffset =
                    VkOffset3D{.x = 0, .y = 0, .z = 0}, // Offset into image (as opposed to raw data in bufferOffset)
                .imageExtent = VkExtent3D{.width = width, .height = height, .depth = 1}
                // Size of region to copy as (x, y, z) values
            };

            // Copy buffer to given image
            vkCmdCopyBufferToImage(transferComandBuffer.getBuffers()[0], srcBuffer, image,
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageRegion);

            transferComandBuffer.end(0);
            SubmitQueue(transferQueue, transferComandBuffer.getBuffers()[0]);
        }

        void TransitionImageLayout(VkDevice device, VkQueue queue, VkCommandPool commandPool, VkImage image,
                                   VkImageLayout oldLayout, VkImageLayout newLayout) {
            // Create buffer
            CommandBuffer commandBuffer(device, commandPool, 1);
            commandBuffer.begin(0, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

            VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_NONE;
            VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_NONE;

            // if transitioning from new image to image ready to receive data..
            VkAccessFlags srcAccessMask = 0;                            // Memory access stage transition must after ..
            VkAccessFlags dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; // Memory access stage transition must before ..

            if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {

                srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

            } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                       newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {

                // if transition from transfer destination to shade readable..
                srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            }

            const VkImageMemoryBarrier imageMemoryBarrier{
                .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                .srcAccessMask = srcAccessMask,
                .dstAccessMask = dstAccessMask,
                .oldLayout = oldLayout,                         // Layout to transition from
                .newLayout = newLayout,                         // layout to transition to
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, // Queue Falmily to transition from
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, // Queue Family to transition to
                .image = image,                                 // Image being accessd and modified as part of barrier
                .subresourceRange = VkImageSubresourceRange{
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, // aspect of image being altered
                    .baseMipLevel = 0,                       // First mip level to start alterations on
                    .levelCount = 1,                         // Number of mip levels to alter starting from maseMipLevel
                    .baseArrayLayer = 0,                     // First layer to start aterarion on
                    .layerCount = 1                          // Number of layers to alter starting from baseArrayLayer
                }};

            vkCmdPipelineBarrier(commandBuffer.getBuffers()[0], //
                                 srcStage, dstStage,            // Pipelane stages (match to src and dst AccessMask)
                                 0,                             // Dependency flags
                                 0, nullptr,                    // Memory Barrier cont + data
                                 0, nullptr,                    // Buffer Memory Barrier cont + data
                                 1, &imageMemoryBarrier         // Image Memory Barrier cont + data
            );

            commandBuffer.end(0);
            SubmitQueue(queue, commandBuffer.getBuffers()[0]);
        }
    } // namespace aux

#pragma endregion

#pragma region IBO

    IBO::IBO(VkPhysicalDevice physical, VkDevice logical) : physical(physical), logical(logical) {}

    IBO::~IBO() { this->destroy(); }

    void IBO::destroy() {
        this->count = 0;
        this->ibo.reset();
    }

    void IBO::create(VkQueue queue, VkCommandPool commandBuffer, std::vector<uint32_t>* indices) {

        // Get size of buffer needed for indices
        VkDeviceSize bufferSize = sizeof(uint32_t) * indices->size();

        this->count = indices->size();

        // Temporary buffer to "stage" index data before transfering to GPU
        BufferObject stagingBuffer(physical, logical);
        stagingBuffer.create(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // MAP MEMORY TO INDEX BUFFER
        stagingBuffer.mapper(indices->data());

        // Create buffer for index data on GPU aceess only area
        this->ibo = std::make_shared<BufferObject>(physical, logical);
        this->ibo->create(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Copy from staging buffer to GPU access buffer
        aux::CopyBuffer(this->logical, queue, commandBuffer, stagingBuffer.getBuffer(), this->ibo->getBuffer(),
                        bufferSize);
    }

#pragma endregion

} // namespace ce
