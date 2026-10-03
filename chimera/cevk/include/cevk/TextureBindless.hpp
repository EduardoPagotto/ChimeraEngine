#pragma once
#include "DescriptorPool.hpp"
#include "Sampler.hpp"
#include "UBO.hpp"
#include "VulkanContext.hpp"
#include "VulkanTexture.hpp"

namespace ce {
    class TextureBindless {
      public:
        explicit TextureBindless(std::shared_ptr<VulkanContext> ctx);
        virtual ~TextureBindless();

        UniformSampler& get_uniform_sampler() { return uniform_; }
        uint32_t alloc_texture(std::shared_ptr<VulkanTexture> tex);

      private:
        Sampler tex_sampler_;
        UniformSampler uniform_;
        DescriptorPool descriptor_pool_;
        std::shared_ptr<VulkanContext> ctx_;
    };
} // namespace ce
