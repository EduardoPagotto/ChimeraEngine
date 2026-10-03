#pragma once

#include "VulkanContext.hpp"
#include <array>
#include <memory>
#include <vulkan/vulkan_core.h>

namespace ce {

    class RenderPass {
      public:
        explicit RenderPass() = default;
        explicit RenderPass(std::shared_ptr<VulkanContext> ctx, VkFormat& format) { init(ctx, format); }

        virtual ~RenderPass() { destroy(); }

        // Proibir cópia (Padrão RAII)
        RenderPass(const RenderPass&) = delete;
        RenderPass& operator=(const RenderPass&) = delete;

        void init(std::shared_ptr<VulkanContext> ctx, const VkFormat& format) {

            ctx_ = ctx;

            // ATTACHEMNTS
            // Colour attachment of render pass
            // Framebuffer data will be storage as an image, but images can be given different data layouts
            // to give optimal use for certan operations
            const VkAttachmentDescription colour_attachemnt{
                .format = format,                        // Format to use for attachment
                .samples = VK_SAMPLE_COUNT_1_BIT,        // Number of samples to write for multisampling
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,   // Describes what to do with attachemnt before rendering
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE, // Describes what todo with attachment after rendering
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,   // Describes what todo with stencil before rendering
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE, // Describes what todo with stencil before rendering
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,         // Image data layout before render pass starts
                .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, // Image data layout after render pass (to change to)
            };

            // Depth attachemnt of render pass
            const VkAttachmentDescription depth_attachemnt{
                .format = VulkanContext::choose_supported_format(
                    ctx->physical,
                    {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT}, // Formats
                    VK_IMAGE_TILING_OPTIMAL,                                                           // Tilling
                    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT),
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

            // REFERENCES
            // Attachemnt reference uses an attachemnt index that refer to index in the attachemnt list passes to
            // renderPassCreateInfo
            const VkAttachmentReference colour_attachment_reference{.attachment = 0,
                                                                    .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

            // Depth Attachment Refence
            const VkAttachmentReference depth_attachemnt_reference{
                .attachment = 1, .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

            // Information about a particular subpass the render pass is using
            const VkSubpassDescription subpass{
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS, // Pipeline type subpass is to be bound to
                .colorAttachmentCount = 1,
                .pColorAttachments = &colour_attachment_reference,
                .pDepthStencilAttachment = &depth_attachemnt_reference};

            // Need to determine when layout transitions occour subpass dependencies
            std::array<VkSubpassDependency, 2> subpass_dependencies;

            // Conversion from VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_COLOR_ATTACHEMNT_OPTIMAL
            // Transition must happen after..
            subpass_dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL; // Subpass index (VK_SUBPASS_EXTERNAL = Special
                                                                      // value means outside of renderpass)
            subpass_dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; // Pipeline stage
            subpass_dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT; // Stage access mask (memory access)

            // But must happen before..
            subpass_dependencies[0].dstSubpass = 0;
            subpass_dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpass_dependencies[0].dstAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            subpass_dependencies[0].dependencyFlags = 0;

            //
            // -----
            // Conversion from VK_IMAGE_LAYOUT_COLOR_ATTACHEMNT_OPTIMAL to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
            // Transition must happen after..
            subpass_dependencies[1].srcSubpass = 0;
            subpass_dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpass_dependencies[1].srcAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            // But must happen before..
            subpass_dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
            subpass_dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            subpass_dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            subpass_dependencies[1].dependencyFlags = 0;

            std::array<VkAttachmentDescription, 2> render_pass_attachemnts = {colour_attachemnt, depth_attachemnt};

            // Create Info for render pass
            const VkRenderPassCreateInfo render_pass_create_info{
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                .attachmentCount = static_cast<uint32_t>(render_pass_attachemnts.size()),
                .pAttachments = render_pass_attachemnts.data(),
                .subpassCount = 1,
                .pSubpasses = &subpass,
                .dependencyCount = static_cast<uint32_t>(subpass_dependencies.size()),
                .pDependencies = subpass_dependencies.data()};

            if (vkCreateRenderPass(ctx->logical, &render_pass_create_info, nullptr, &render_pass_) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create render pass!!!");
            }
        }

        void destroy() {
            if (render_pass_ != VK_NULL_HANDLE) {
                vkDestroyRenderPass(ctx_->logical, render_pass_, nullptr);
                render_pass_ = VK_NULL_HANDLE;
            }
        }

        VkRenderPass& get_render_pass() { return render_pass_; }

        static VkResult send_image_to_screen(VkQueue p_queue, VkSemaphore signal, VkSwapchainKHR swapchain,
                                             uint32_t& image_index) {
            //
            // -- PRESENT RENDERED IMAGE TO SCREEN --
            std::array<VkSemaphore, 1> signal_semaphores{signal};
            std::array<VkSwapchainKHR, 1> swap_chains{swapchain};

            const VkPresentInfoKHR present_info{
                .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                .waitSemaphoreCount =
                    static_cast<uint32_t>(signal_semaphores.size()),         // Number of semaphores to wait on
                .pWaitSemaphores = signal_semaphores.data(),                 // Semaphores to wait on
                .swapchainCount = static_cast<uint32_t>(swap_chains.size()), // Number of swapchains to present to
                .pSwapchains = swap_chains.data(),                           // Swapchais to present images to
                .pImageIndices = &image_index,                               // Index of Images in swapchains to present
            };

            // Present Image
            return vkQueuePresentKHR(p_queue, &present_info);
        }

      private:
        std::shared_ptr<VulkanContext> ctx_{nullptr};
        VkRenderPass render_pass_{VK_NULL_HANDLE};
    };

} // namespace ce
