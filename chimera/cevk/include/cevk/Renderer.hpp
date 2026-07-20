#pragma once

#include "cevk.hpp"

namespace ce {

    class Renderer {
      public:
        explicit Renderer(BaseVK* pBVK, const VkFormat& format);
        virtual ~Renderer();

        VkRenderPass& getRenderPass() { return renderPass; }

        void passBegin(VkFramebuffer& framebuffer, VkRenderPassBeginInfo* r) {

            r->sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            r->renderPass = this->renderPass;                               // Render pass to begin
            r->framebuffer = framebuffer;                                   //
            r->renderArea = this->renderArea;                               //
            r->clearValueCount = static_cast<uint32_t>(clearValues.size()); //
            r->pClearValues = clearValues.data();                           // List of clear values
        }

        void setRederArea(const VkRect2D& renderArea) { this->renderArea = renderArea; }

      private:
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice logical{VK_NULL_HANDLE};

        VkRenderPass renderPass;

        void createRenderPass(const VkFormat& format);

        std::vector<VkClearValue> clearValues;
        VkRect2D renderArea;
    };
} // namespace ce
