#include "CmdRender.hpp"
#include <array>
#include <stdexcept>

namespace ce {

    void CmdRender::begin(VkCommandBuffer cmdbuffer, VkCommandBufferUsageFlagBits flag,
                          const VkRenderPassBeginInfo& renderpass_begin_info, VkPipeline& graphic_pipeline) {

        this->cmdbuffer_ = cmdbuffer;
        // Information to begin the command buffer record
        const VkCommandBufferBeginInfo begin_info{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = flag // We're only using the command buffer once, so set up for one time submit
        };

        // Begin recording transfer commands
        if (vkBeginCommandBuffer(this->cmdbuffer_, &begin_info) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin a CmdRender Buffer!");
        }

        // Begin Render Pass
        vkCmdBeginRenderPass(this->cmdbuffer_, &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        // Bind Pipeline to be used  in render pass
        vkCmdBindPipeline(this->cmdbuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, graphic_pipeline);
    }

    void CmdRender::pushConstants(VkPipelineLayout pipeline_layout, VkShaderStageFlagBits stage, uint32_t offset,
                                  size_t size, const void* src) {
        // "Push" constant to given shader stage directly (no buffer)
        vkCmdPushConstants(cmdbuffer_,      //
                           pipeline_layout, //
                           stage,           // Stage to push constant to
                           offset,          // offset of pushconstant to update
                           size,            // size of data being pushed
                           src);            // Actual data being pushed (cam be array)
    }

    void CmdRender::addVertexBuffer(const VkDeviceSize& offset, const VkBuffer& buffer) {
        this->vextex_buffers_.push_back(buffer);
        this->offsets_.push_back(offset);
    }

    void CmdRender::bindIndexBuffer(const VkDeviceSize& offset, const VkBuffer& index_buffer) { // TODO: offset {0}
        // Bind mesh index buffer, with 0 offset and using the uint32_t type
        vkCmdBindIndexBuffer(cmdbuffer_, index_buffer, offset, VK_INDEX_TYPE_UINT32);
    }

    void CmdRender::bindVertexBuffer(uint32_t starts) { // TODO: inicia com 0
        vkCmdBindVertexBuffers(cmdbuffer_, starts, static_cast<uint32_t>(vextex_buffers_.size()),
                               vextex_buffers_.data(),
                               offsets_.data()); // CmdRender to bind vertex buffer before drawing with then
    }

    void CmdRender::addDescriptorSet(const VkDescriptorSet& desc) { this->descriptorset_group_.push_back(desc); }

    void CmdRender::bindDescriptorSets(const VkPipelineLayout& pipeline_layout) {
        vkCmdBindDescriptorSets(cmdbuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0,
                                static_cast<uint32_t>(descriptorset_group_.size()), descriptorset_group_.data(), 0,
                                nullptr);
    }

    void CmdRender::drawIndexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index,
                                int32_t vertex_offset, uint32_t first_instance) {

        vkCmdDrawIndexed(cmdbuffer_, index_count, instance_count, first_index, vertex_offset, first_instance);
    }

    void CmdRender::end() {
        vkCmdEndRenderPass(this->cmdbuffer_);

        if (vkEndCommandBuffer(this->cmdbuffer_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to end a CmdRender!");
        }
    }

    void CmdRender::clearTemps() {
        vextex_buffers_.clear();
        offsets_.clear();
        descriptorset_group_.clear();
    }

    void CmdRender::submitToRender(VkQueue queue, Frame* frame, const VkPipelineStageFlagBits& pipeline_stage_flags) {
        // -- SUBMIT COMMAND BUFFER TO RENDER
        // Queue submission information
        std::array<VkSemaphore, 1> wait_semaphores{frame->imageAvailableSemaphore};   // sync.getWait()};
        std::array<VkSemaphore, 1> signal_semaphores{frame->renderFinishedSemaphore}; // sync.getSignal()};
        std::array<VkPipelineStageFlags, 1> wait_stages{pipeline_stage_flags};

        const VkSubmitInfo submit_info{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = static_cast<uint32_t>(wait_semaphores.size()), // Number of semaphores to wait on
            .pWaitSemaphores = wait_semaphores.data(),                           //
            .pWaitDstStageMask = wait_stages.data(),                             // Stagegs to check semaphores at
            .commandBufferCount = 1,              // Number of command buffers to submit FIXME: é isto mesmo?
            .pCommandBuffers = &this->cmdbuffer_, // Command buffer to submit
            .signalSemaphoreCount = static_cast<uint32_t>(signal_semaphores.size()), // Number of semaphore to signal
            .pSignalSemaphores = signal_semaphores.data(), // Semaphore to signal when command buffer finishes
        };

        // Submit command buffer to queue
        if (vkQueueSubmit(queue, 1, &submit_info, frame->inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit Command Buffer to Queue!");
        }
    }
} // namespace ce
