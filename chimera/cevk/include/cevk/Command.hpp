#ifndef COMMAND_CLASS_HPP
#define COMMAND_CLASS_HPP

#include <array>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace ce {

    struct SubmitToRenderInfo {
        VkQueue gQueue;
        VkSemaphore wait;
        VkSemaphore signal;
        VkFence fence;
        VkPipelineStageFlagBits pipelineStageFlags;
        // size_t bufferIndex;
    };

    class Command {
      public:
        explicit Command(VkCommandBuffer cmdBuffer, VkCommandBufferUsageFlagBits flag) : cmdBuffer(cmdBuffer) {

            // Information to begin the command buffer record
            const VkCommandBufferBeginInfo beginInfo{
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .flags = flag // We're only using the command buffer once, so set up for one time submit
            };

            // Begin recording transfer commands
            if (vkBeginCommandBuffer(cmdBuffer, &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("Failed to begin a Command Buffer!");
            }
        }

        virtual ~Command() { this->destroy(); }

        Command(const Command&) = delete;
        Command& operator=(const Command&) = delete;

        void destroy() {
            if (cmdBuffer != VK_NULL_HANDLE) {
                if (vkEndCommandBuffer(cmdBuffer) != VK_SUCCESS) {
                    throw std::runtime_error("Failed to end a Command Buffer!");
                }
            }
        }

        void beginAndPipeline(const VkRenderPassBeginInfo& renderPassBeginInfo, VkPipeline& graphicPipeline) {
            // Begin Render Pass
            vkCmdBeginRenderPass(this->cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            // Bind Pipeline to be used  in render pass
            vkCmdBindPipeline(this->cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline);
        }

        void pushConstants(VkPipelineLayout pipelineLayout, VkShaderStageFlagBits stage, uint32_t offset, size_t size,
                           const void* src) {
            // "Push" constant to given shader stage directly (no buffer)
            vkCmdPushConstants(cmdBuffer,      //
                               pipelineLayout, //
                               stage,          // Stage to push constant to
                               offset,         // offset of pushconstant to update
                               size,           // size of data being pushed
                               src);           // Actual data being pushed (cam be array)
        }

        void addVertexBuffer(const VkDeviceSize& offset, const VkBuffer& buffer) {
            this->vextexBuffers.push_back(buffer);
            this->offsets.push_back(offset);
        }

        void bindIndexBuffer(const VkBuffer& indexBuffer, const VkDeviceSize& offset) { // TODO: offset {0}
            // Bind mesh index buffer, with 0 offset and using the uint32_t type
            vkCmdBindIndexBuffer(cmdBuffer, indexBuffer, offset, VK_INDEX_TYPE_UINT32);
        }

        void bindVertexBuffer(uint32_t starts) { // TODO: inicia com 0
            vkCmdBindVertexBuffers(cmdBuffer, starts, static_cast<uint32_t>(vextexBuffers.size()), vextexBuffers.data(),
                                   offsets.data()); // Command to bind vertex buffer before drawing with then
        }

        void addDescriptorSet(const VkDescriptorSet& desc) { this->descriptorSetGroup.push_back(desc); }

        void bindDescriptorSets(const VkPipelineLayout& pipelineLayout) {
            vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                    static_cast<uint32_t>(descriptorSetGroup.size()), descriptorSetGroup.data(), 0,
                                    nullptr);
        }

        void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
                         uint32_t firstInstance) {

            vkCmdDrawIndexed(cmdBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        }

        void end() { vkCmdEndRenderPass(cmdBuffer); }

        void clearTemps() {
            vextexBuffers.clear();
            offsets.clear();
            descriptorSetGroup.clear();
        }

        static void SubmitToRender(const SubmitToRenderInfo& sub, VkCommandBuffer& cmdBuffer) {
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
                .commandBufferCount = 1,       // Number of command buffers to submit FIXME: é isto mesmo?
                .pCommandBuffers = &cmdBuffer, // Command buffer to submit
                .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()), // Number of semaphore to signal
                .pSignalSemaphores = signalSemaphores.data(), // Semaphore to signal when command buffer finishes
            };

            // Submit command buffer to queue
            if (vkQueueSubmit(sub.gQueue, 1, &submitInfo, sub.fence) != VK_SUCCESS) {
                throw std::runtime_error("Failed to submit Command Buffer to Queue!");
            }
        }

      private:
        std::vector<VkBuffer> vextexBuffers;
        std::vector<VkDeviceSize> offsets;
        std::vector<VkDescriptorSet> descriptorSetGroup;

        VkCommandBuffer cmdBuffer{VK_NULL_HANDLE};
    };
} // namespace ce

#endif
