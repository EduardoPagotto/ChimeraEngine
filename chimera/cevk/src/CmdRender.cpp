#include "CmdRender.hpp"
#include <array>
#include <stdexcept>

namespace ce {

    void CmdRender::begin(VkCommandBuffer cmdBuffer, VkCommandBufferUsageFlagBits flag,
                          const VkRenderPassBeginInfo& renderPassBeginInfo, VkPipeline& graphicPipeline) {

        this->cmdBuffer = cmdBuffer;
        // Information to begin the command buffer record
        const VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = flag // We're only using the command buffer once, so set up for one time submit
        };

        // Begin recording transfer commands
        if (vkBeginCommandBuffer(this->cmdBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin a CmdRender Buffer!");
        }

        // Begin Render Pass
        vkCmdBeginRenderPass(this->cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Bind Pipeline to be used  in render pass
        vkCmdBindPipeline(this->cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline);
    }

    void CmdRender::pushConstants(VkPipelineLayout pipelineLayout, VkShaderStageFlagBits stage, uint32_t offset,
                                  size_t size, const void* src) {
        // "Push" constant to given shader stage directly (no buffer)
        vkCmdPushConstants(cmdBuffer,      //
                           pipelineLayout, //
                           stage,          // Stage to push constant to
                           offset,         // offset of pushconstant to update
                           size,           // size of data being pushed
                           src);           // Actual data being pushed (cam be array)
    }

    void CmdRender::addVertexBuffer(const VkDeviceSize& offset, const VkBuffer& buffer) {
        this->vextexBuffers.push_back(buffer);
        this->offsets.push_back(offset);
    }

    void CmdRender::bindIndexBuffer(const VkBuffer& indexBuffer, const VkDeviceSize& offset) { // TODO: offset {0}
        // Bind mesh index buffer, with 0 offset and using the uint32_t type
        vkCmdBindIndexBuffer(cmdBuffer, indexBuffer, offset, VK_INDEX_TYPE_UINT32);
    }

    void CmdRender::bindVertexBuffer(uint32_t starts) { // TODO: inicia com 0
        vkCmdBindVertexBuffers(cmdBuffer, starts, static_cast<uint32_t>(vextexBuffers.size()), vextexBuffers.data(),
                               offsets.data()); // CmdRender to bind vertex buffer before drawing with then
    }

    void CmdRender::addDescriptorSet(const VkDescriptorSet& desc) { this->descriptorSetGroup.push_back(desc); }

    void CmdRender::bindDescriptorSets(const VkPipelineLayout& pipelineLayout) {
        vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                static_cast<uint32_t>(descriptorSetGroup.size()), descriptorSetGroup.data(), 0,
                                nullptr);
    }

    void CmdRender::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
                                uint32_t firstInstance) {

        vkCmdDrawIndexed(cmdBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    void CmdRender::end() {
        vkCmdEndRenderPass(this->cmdBuffer);

        if (vkEndCommandBuffer(this->cmdBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to end a CmdRender!");
        }
    }

    void CmdRender::clearTemps() {
        vextexBuffers.clear();
        offsets.clear();
        descriptorSetGroup.clear();
    }

    void CmdRender::submitToRender(VkQueue queue, Frame* frame, const VkPipelineStageFlagBits& pipelineStageFlags) {
        // -- SUBMIT COMMAND BUFFER TO RENDER
        // Queue submission information
        std::array<VkSemaphore, 1> waitSemaphores{frame->imageAvailableSemaphore};   // sync.getWait()};
        std::array<VkSemaphore, 1> signalSemaphores{frame->renderFinishedSemaphore}; // sync.getSignal()};
        std::array<VkPipelineStageFlags, 1> waitStages{pipelineStageFlags};

        const VkSubmitInfo submitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()), // Number of semaphores to wait on
            .pWaitSemaphores = waitSemaphores.data(),                           //
            .pWaitDstStageMask = waitStages.data(),                             // Stagegs to check semaphores at
            .commandBufferCount = 1,             // Number of command buffers to submit FIXME: é isto mesmo?
            .pCommandBuffers = &this->cmdBuffer, // Command buffer to submit
            .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()), // Number of semaphore to signal
            .pSignalSemaphores = signalSemaphores.data(), // Semaphore to signal when command buffer finishes
        };

        // Submit command buffer to queue
        if (vkQueueSubmit(queue, 1, &submitInfo, frame->inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit Command Buffer to Queue!");
        }
    }
} // namespace ce
