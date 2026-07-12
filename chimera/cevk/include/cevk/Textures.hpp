#ifndef __TEXTURES_CLASS_HPP
#define __TEXTURES_CLASS_HPP

#include "Buffers.hpp"
#include "stb_image.h"

namespace ce {
    class Textures {

      public:
        explicit Textures(VkPhysicalDevice physical, VkDevice logical);
        virtual ~Textures();

        Textures(const Textures&) = delete;
        Textures& operator=(const Textures&) = delete;

        std::shared_ptr<UBO<ImageObject>> getUbo() { return uboSampler; }

        int createTexture(const std::string& filename, VkQueue queue, VkCommandPool commandPool);

      private:
        void createDescriptorSetLayout();
        void createDescriptorPool();
        void createTextureSampler();
        int createTextureImage(const std::string& filename, VkQueue queue, VkCommandPool commandPool);
        int createTextureDescriptor(VkImageView textureImage);

        static stbi_uc* loadTextureFile(const std::string& filename, int* width, int* height, VkDeviceSize* imageSize);

        VkPhysicalDevice physical;
        VkDevice logical;

        VkSampler textureSampler;

        std::shared_ptr<UBO<ImageObject>> uboSampler;
        std::shared_ptr<DescriptorPool> samplerDescriptorPool;
    };
} // namespace ce
#endif
