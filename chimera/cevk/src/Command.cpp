#include "Command.hpp"
#include <stdexcept>

namespace ce {

    CommandRender::CommandRender(VkCommandBuffer cmdBuffer, VkCommandBufferUsageFlagBits flag) {
        this->init(cmdBuffer, flag);
    }

    CommandRender::~CommandRender() { this->destroy(); }

    void CommandRender::init(VkCommandBuffer cmdBuffer, VkCommandBufferUsageFlagBits flag) {

        this->cmdBuffer = cmdBuffer;
        // Information to begin the command buffer record
        const VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = flag // We're only using the command buffer once, so set up for one time submit
        };

        // Begin recording transfer commands
        if (vkBeginCommandBuffer(this->cmdBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin a CommandRender Buffer!");
        }
    }

    void CommandRender::destroy() {
        if (this->cmdBuffer != VK_NULL_HANDLE) {
            if (vkEndCommandBuffer(this->cmdBuffer) != VK_SUCCESS) {
                throw std::runtime_error("Failed to end a CommandRender Buffer!");
            }
            this->cmdBuffer = VK_NULL_HANDLE;
        }
    }

    void CommandRender::beginAndPipeline(const VkRenderPassBeginInfo& renderPassBeginInfo,
                                         VkPipeline& graphicPipeline) {
        // Begin Render Pass
        vkCmdBeginRenderPass(this->cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Bind Pipeline to be used  in render pass
        vkCmdBindPipeline(this->cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline);
    }

    void CommandRender::pushConstants(VkPipelineLayout pipelineLayout, VkShaderStageFlagBits stage, uint32_t offset,
                                      size_t size, const void* src) {
        // "Push" constant to given shader stage directly (no buffer)
        vkCmdPushConstants(cmdBuffer,      //
                           pipelineLayout, //
                           stage,          // Stage to push constant to
                           offset,         // offset of pushconstant to update
                           size,           // size of data being pushed
                           src);           // Actual data being pushed (cam be array)
    }

    void CommandRender::addVertexBuffer(const VkDeviceSize& offset, const VkBuffer& buffer) {
        this->vextexBuffers.push_back(buffer);
        this->offsets.push_back(offset);
    }

    void CommandRender::bindIndexBuffer(const VkBuffer& indexBuffer, const VkDeviceSize& offset) { // TODO: offset {0}
        // Bind mesh index buffer, with 0 offset and using the uint32_t type
        vkCmdBindIndexBuffer(cmdBuffer, indexBuffer, offset, VK_INDEX_TYPE_UINT32);
    }

    void CommandRender::bindVertexBuffer(uint32_t starts) { // TODO: inicia com 0
        vkCmdBindVertexBuffers(cmdBuffer, starts, static_cast<uint32_t>(vextexBuffers.size()), vextexBuffers.data(),
                               offsets.data()); // CommandRender to bind vertex buffer before drawing with then
    }

    void CommandRender::addDescriptorSet(const VkDescriptorSet& desc) { this->descriptorSetGroup.push_back(desc); }

    void CommandRender::bindDescriptorSets(const VkPipelineLayout& pipelineLayout) {
        vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                static_cast<uint32_t>(descriptorSetGroup.size()), descriptorSetGroup.data(), 0,
                                nullptr);
    }

    void CommandRender::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                                    int32_t vertexOffset, uint32_t firstInstance) {

        vkCmdDrawIndexed(cmdBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    void CommandRender::end() { vkCmdEndRenderPass(cmdBuffer); }

    void CommandRender::clearTemps() {
        vextexBuffers.clear();
        offsets.clear();
        descriptorSetGroup.clear();
    }
} // namespace ce
