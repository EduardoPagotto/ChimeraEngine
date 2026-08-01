#pragma once

#include "DescriptorPool.hpp"
#include "Sampler.hpp"
#include "UBO.hpp"
#include "VulkanContext.hpp"
#include <vulkan/vulkan_core.h>

namespace ce {
    class Textures {
      public:
        explicit Textures(std::shared_ptr<VulkanContext> ctx);
        virtual ~Textures();

        UniformSampler& getUniformSampler() { return uniformSampler; }
        int createTexture(const std::string& filename);

      private:
        int createTextureDescriptor(VkImageView textureImage);

        Sampler texSampler;
        UniformSampler uniformSampler;
        DescriptorPool samplerDescriptorPool;
        std::shared_ptr<VulkanContext> ctx;
    };
} // namespace ce
