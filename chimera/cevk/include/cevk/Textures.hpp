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

        UniformSampler& getUniformSampler() { return uniformSampler; }
        int allocTexture(std::shared_ptr<VulkanTexture> vulkanTex);

      private:
        Sampler texSampler;
        UniformSampler uniformSampler;
        DescriptorPool samplerDescriptorPool;
        std::shared_ptr<VulkanContext> ctx;
    };
} // namespace ce
