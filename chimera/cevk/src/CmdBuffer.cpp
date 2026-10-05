#include "CmdBuffer.hpp"
#include <stdexcept>

namespace ce {
    CmdBuffer::CmdBuffer(VkDevice device, VkCommandPool commandpool) { init(device, commandpool); }

    CmdBuffer::~CmdBuffer() { destroy(); }

    void CmdBuffer::init(VkDevice device, VkCommandPool commandpool) {

        device_ = device;
        commandpool_ = commandpool;

        const VkCommandBufferAllocateInfo cb_alloc_info{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = commandpool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, // VK_COMMAND_BUFFER_LEVEL_PRIMARY : Buffer you submit directly
                                                      // to queue. Can't be called by other buffers.
                                                      // VK_COMMAND_BUFFER_LEVEL_SECUNDARY : Buffer can't be called
                                                      // directly. cam be called from other buffe via
                                                      // "VkCmdExecuteCommand" when recording commands in primary buf
            .commandBufferCount = static_cast<uint32_t>(1)};

        // Allocate command buffers and place handles in array of buffers
        if (vkAllocateCommandBuffers(device, &cb_alloc_info, &handle_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to Allocate Command buffers!");
        }
    }

    void CmdBuffer::destroy() {
        // Free temporary command buffer back to pool
        if (handle_ != VK_NULL_HANDLE) {
            vkFreeCommandBuffers(device_, commandpool_, static_cast<uint32_t>(1), &handle_);
            handle_ = VK_NULL_HANDLE;
        }
    }

    void CmdBuffer::clean() {
        if (vkResetCommandBuffer(handle_, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS) {
            throw std::runtime_error("Failed to reset a Command Buffer!");
        }
    }

    void CmdBuffer::begin(VkCommandBufferUsageFlagBits flag) {

        // Information to begin the command buffer record
        const VkCommandBufferBeginInfo begin_info{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = flag // We're only using the command buffer once, so set up for one time submit
        };

        // Begin recording transfer commands
        if (vkBeginCommandBuffer(handle_, &begin_info) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin a Command Buffer!");
        }
    }

    void CmdBuffer::end() {
        // End commands
        if (vkEndCommandBuffer(handle_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to end a Command Buffer!");
        }
    }

    void CmdBuffer::submit_queue(VkQueue queue) {
        // Queue submission information
        const VkSubmitInfo submit_info{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO, //
            .commandBufferCount = 1,                //
            .pCommandBuffers = &handle_             //
        };

        // Submit transfer command to transfer queue and wait until it finishes
        vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);
    }

    namespace aux {

        void copy_buffer(VkDevice device, VkQueue queue, VkCommandPool commandpool, VkBuffer src_buffer,
                         VkBuffer dst_buffer, VkDeviceSize buffer_size) {

            CmdBuffer cmdbuffer(device, commandpool);
            cmdbuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

            // Region of data to copy from and to
            const VkBufferCopy buffer_copy_region{.srcOffset = 0, .dstOffset = 0, .size = buffer_size};

            // Command to copy src buffer to dst buffer
            vkCmdCopyBuffer(cmdbuffer.get(), src_buffer, dst_buffer, 1, &buffer_copy_region);

            cmdbuffer.end();
            cmdbuffer.submit_queue(queue);
        }

        void copy_image_buffer(VkDevice device, VkQueue queue, VkCommandPool commandpool, VkBuffer src_buffer,
                               VkImage image, uint32_t width, uint32_t height) {
            // Create Buffer
            CmdBuffer cmdbuffer(device, commandpool);
            cmdbuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

            const VkBufferImageCopy image_region{
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
            vkCmdCopyBufferToImage(cmdbuffer.get(), src_buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
                                   &image_region);

            cmdbuffer.end();
            cmdbuffer.submit_queue(queue);
        }

        void transition_image_layout(VkDevice device, VkQueue queue, VkCommandPool commandpool, VkImage image,
                                     VkImageLayout old_layout, VkImageLayout new_layout) {
            // Create buffer
            CmdBuffer cmdbuffer(device, commandpool);
            cmdbuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

            VkPipelineStageFlags src_stage = VK_PIPELINE_STAGE_NONE;
            VkPipelineStageFlags dst_stage = VK_PIPELINE_STAGE_NONE;

            // if transitioning from new image to image ready to receive data..
            VkAccessFlags src_access_mask = 0; // Memory access stage transition must after ..
            VkAccessFlags dst_access_mask =
                VK_ACCESS_TRANSFER_WRITE_BIT; // Memory access stage transition must before ..

            if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {

                src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;

            } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                       new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {

                // if transition from transfer destination to shade readable..
                src_access_mask = VK_ACCESS_TRANSFER_WRITE_BIT;
                dst_access_mask = VK_ACCESS_SHADER_READ_BIT;

                src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            }

            const VkImageMemoryBarrier image_memory_barrier{
                .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                .srcAccessMask = src_access_mask,
                .dstAccessMask = dst_access_mask,
                .oldLayout = old_layout,                        // Layout to transition from
                .newLayout = new_layout,                        // layout to transition to
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

            vkCmdPipelineBarrier(cmdbuffer.get(),         //
                                 src_stage, dst_stage,    // Pipelane stages (match to src and dst AccessMask)
                                 0,                       // Dependency flags
                                 0, nullptr,              // Memory Barrier cont + data
                                 0, nullptr,              // Buffer Memory Barrier cont + data
                                 1, &image_memory_barrier // Image Memory Barrier cont + data
            );

            cmdbuffer.end();
            cmdbuffer.submit_queue(queue);
        }
    } // namespace aux
} // namespace ce
