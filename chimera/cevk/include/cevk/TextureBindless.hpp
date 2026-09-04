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

        UniformSampler& getUniformSampler() { return uniform; }
        uint32_t allocTexture(std::shared_ptr<VulkanTexture> tex);

      private:
        Sampler texSampler;
        UniformSampler uniform;
        DescriptorPool descriptorPool;
        std::shared_ptr<VulkanContext> ctx;
    };
} // namespace ce
