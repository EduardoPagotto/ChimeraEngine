#pragma once

#include <stdexcept>
#include <vulkan/vulkan_core.h>
namespace ce {
    class Sampler {
      public:
        explicit Sampler() = default;
        explicit Sampler(VkDevice logical) { this->init(logical); }
        virtual ~Sampler() { this->destroy(); }

        void init(VkDevice logical) {
            this->logical = logical;
            this->create();
        }

        void destroy() {
            if (textureSampler != VK_NULL_HANDLE) {
                vkDestroySampler(this->logical, this->textureSampler, nullptr);
                textureSampler = VK_NULL_HANDLE;
            }
        }

        VkSampler& get() { return this->textureSampler; }

      private:
        void create() {
            //------------------------------------------------------------------------------------
            //  CREATE TEXTURE SAMPLER
            //------------------------------------------------------------------------------------
            const VkSamplerCreateInfo samplerCreateInfo{
                .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
                .magFilter = VK_FILTER_LINEAR,                   // How torender when image is magnified on screen
                .minFilter = VK_FILTER_LINEAR,                   // How to render when image is minifield on screen
                .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,     // Mipmap interpolation mode
                .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,  // texture wrap in U(x) direction wrap_s
                .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,  // texture wrap in V(y) direction wrap_t
                .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,  // texture wrap in W(z) direction wrap_r
                .mipLodBias = 0.0F,                              // Level of detail of bias for mip level
                .anisotropyEnable = VK_TRUE,                     // Enable anisotropy
                .maxAnisotropy = 16,                             // Anisotropy sample level
                .minLod = 0.0F,                                  // Minimum Level Detail ro pick mip level
                .maxLod = 0.0F,                                  // Maximum Level Detail ro pick mip level
                .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK, // Border beond texture (only works for border clamp)
                .unnormalizedCoordinates = VK_FALSE,             // Wheter coords should be normalized (between 0 and 1)
            };

            if (vkCreateSampler(this->logical, &samplerCreateInfo, nullptr, &this->textureSampler) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create a Sampler");
            }
        }

        VkDevice logical{VK_NULL_HANDLE};
        VkSampler textureSampler{VK_NULL_HANDLE};
    };
} // namespace ce
