#include "Renderer.hpp"
#include "DevVK.hpp"
#include <array>
#include <stdexcept>

namespace ce {

    Renderer::Renderer(std::shared_ptr<BaseVK> bvk, const VkFormat& format) : bvk(bvk) { // NOLINT
        //
        createRenderPass(format);
    }
    Renderer::~Renderer() { vkDestroyRenderPass(bvk->logical, this->renderPass, nullptr); }

    void Renderer::createRenderPass(const VkFormat& format) {

        // ATTACHEMNTS
        // Colour attachment of render pass
        // Framebuffer data will be storage as an image, but images can be given different data layouts
        // to give optimal use for certan operations
        const VkAttachmentDescription colourAttachemnt{
            .format = format,                                   // Format to use for attachment
            .samples = VK_SAMPLE_COUNT_1_BIT,                   // Number of samples to write for multisampling
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,              // Describes what to do with attachemnt before rendering
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,            // Describes what todo with attachment after rendering
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,   // Describes what todo with stencil before rendering
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE, // Describes what todo with stencil before rendering
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,         // Image data layout before render pass starts
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,     // Image data layout after render pass (to change to)
        };

        // Depth attachemnt of render pass
        const VkAttachmentDescription depthAttachemnt{
            .format = aux::ChooseSupportedFormat(
                this->bvk->physical,
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
        const VkAttachmentReference colourAttachmentReference{.attachment = 0,
                                                              .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        // Depth Attachment Refence
        const VkAttachmentReference depthAttachemntReference{
            .attachment = 1, .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

        // Information about a particular subpass the render pass is using
        const VkSubpassDescription subpass{
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS, // Pipeline type subpass is to be bound to
            .colorAttachmentCount = 1,
            .pColorAttachments = &colourAttachmentReference,
            .pDepthStencilAttachment = &depthAttachemntReference};

        // Need to determine when layout transitions occour subpass dependencies
        std::array<VkSubpassDependency, 2> subpassDependencies;

        // Conversion from VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_COLOR_ATTACHEMNT_OPTIMAL
        // Transition must happen after..
        subpassDependencies[0].srcSubpass =
            VK_SUBPASS_EXTERNAL; // Subpass index (VK_SUBPASS_EXTERNAL = Special value means outside of renderpass)
        subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; // Pipeline stage
        subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;           // Stage access mask (memory access)

        // But must happen before..
        subpassDependencies[0].dstSubpass = 0;
        subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependencies[0].dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependencies[0].dependencyFlags = 0;

        //
        // -----
        // Conversion from VK_IMAGE_LAYOUT_COLOR_ATTACHEMNT_OPTIMAL to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        // Transition must happen after..
        subpassDependencies[1].srcSubpass = 0;
        subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependencies[1].srcAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        // But must happen before..
        subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        subpassDependencies[1].dependencyFlags = 0;

        std::array<VkAttachmentDescription, 2> renderPassAttachemnts = {colourAttachemnt, depthAttachemnt};

        // Create Info for render pass
        const VkRenderPassCreateInfo renderPassCreateInfo{
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = static_cast<uint32_t>(renderPassAttachemnts.size()),
            .pAttachments = renderPassAttachemnts.data(),
            .subpassCount = 1,
            .pSubpasses = &subpass,
            .dependencyCount = static_cast<uint32_t>(subpassDependencies.size()),
            .pDependencies = subpassDependencies.data()};

        if (vkCreateRenderPass(bvk->logical, &renderPassCreateInfo, nullptr, &this->renderPass) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create render pass!!!");
        }
    }
} // namespace ce
