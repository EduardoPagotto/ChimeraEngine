#pragma once

#include "DescriptorPool.hpp"
#include "Sampler.hpp"
#include "UBO.hpp"
#include "VulkanContext.hpp"
#include "VulkanTexture.hpp"
#include <vulkan/vulkan_core.h>

namespace ce {
    class Textures {
      public:
        explicit Textures(std::shared_ptr<VulkanContext> ctx);
        virtual ~Textures();

        UniformSampler& getUniformSampler() { return uniform; }
        uint32_t allocTexture(std::shared_ptr<VulkanTexture> vulkanTex);

      private:
        Sampler texSampler;
        UniformSampler uniform;
        DescriptorPool descriptorPool;
        std::shared_ptr<VulkanContext> ctx;
    };
} // namespace ce
