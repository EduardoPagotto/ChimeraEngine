#pragma once

#include "DescriptorPool.hpp"
#include "UBO.hpp"
#include "VulkanContext.hpp"
#include <vulkan/vulkan_core.h>

namespace ce {
    class Textures {

      public:
        explicit Textures(VulkanContext& context);
        virtual ~Textures();

        Textures(const Textures&) = delete;
        Textures& operator=(const Textures&) = delete;

        UniformSampler& getUniformSampler() { return uniformSampler; }
        int createTexture(const std::string& filename);

      private:
        void createDescriptorSetLayout();
        void createDescriptorPool();
        void createTextureSampler();
        int createTextureImage(const std::string& filename);
        int createTextureDescriptor(VkImageView textureImage);

        VulkanContext& context;
        VkSampler textureSampler{VK_NULL_HANDLE};
        UniformSampler uniformSampler;
        DescriptorPool samplerDescriptorPool;
    };
} // namespace ce
