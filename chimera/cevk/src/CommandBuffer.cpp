#include "CommandBuffer.hpp"
#include <array>
#include <stdexcept>

namespace ce {
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

    void CommandBuffer::submitToRender(const SubmitToRenderInfo& sub, size_t index) {
        // -- SUBMIT COMMAND BUFFER TO RENDER
        // Queue submission information
        std::array<VkSemaphore, 1> waitSemaphores{sub.wait};
        std::array<VkSemaphore, 1> signalSemaphores{sub.signal};
        std::array<VkPipelineStageFlags, 1> waitStages{
            sub.pipelineStageFlags}; //{VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        const VkSubmitInfo submitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()), // Number of semaphores to wait on
            .pWaitSemaphores = waitSemaphores.data(),                           //
            .pWaitDstStageMask = waitStages.data(),                             // Stagegs to check semaphores at
            .commandBufferCount = 1,                         // Number of command buffers to submit FIXME: é isto mesmo?
            .pCommandBuffers = &this->commandBuffers[index], // cmdBuffer, // Command buffer to submit
            .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()), // Number of semaphore to signal
            .pSignalSemaphores = signalSemaphores.data(), // Semaphore to signal when command buffer finishes
        };

        // Submit command buffer to queue
        if (vkQueueSubmit(sub.gQueue, 1, &submitInfo, sub.fence) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit Command Buffer to Queue!");
        }
    }

    void CommandBuffer::submitQueue(VkQueue queue, size_t index) {
        // Queue submission information
        const VkSubmitInfo submitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,         //
            .commandBufferCount = 1,                        //
            .pCommandBuffers = &this->commandBuffers[index] //
        };

        // Submit transfer command to transfer queue and wait until it finishes
        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);
    }

    namespace aux {

        void CopyBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, VkBuffer srcBuffer,
                        VkBuffer dstBuffer, VkDeviceSize bufferSize) {

            CommandBuffer transferComandBuffer(device, transferCommandPool, 1);
            transferComandBuffer.begin(0, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

            // Region of data to copy from and to
            const VkBufferCopy bufferCopyRegion{.srcOffset = 0, .dstOffset = 0, .size = bufferSize};

            // Command to copy src buffer to dst buffer
            vkCmdCopyBuffer(transferComandBuffer.getBuffers()[0], srcBuffer, dstBuffer, 1, &bufferCopyRegion);

            transferComandBuffer.end(0);
            transferComandBuffer.submitQueue(transferQueue, 0);
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
            transferComandBuffer.submitQueue(transferQueue, 0);
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
            commandBuffer.submitQueue(queue, 0);
        }
    } // namespace aux

} // namespace ce
