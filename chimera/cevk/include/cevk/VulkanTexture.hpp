#pragma once

#include "Image.hpp"
#include "VulkanContext.hpp"
#include <SDL3_image/SDL_image.h>
#include <memory>

namespace ce {

    class VulkanTexture {

      public:
        VulkanTexture(std::shared_ptr<Image> texImg) : texImg(texImg), index(nextTextureIndex++) {}
        ~VulkanTexture() { texImg.reset(); }

        // Factory pattern exigido pelo AssetManager
        static std::shared_ptr<VulkanTexture> create(std::shared_ptr<VulkanContext> ctx, const std::string& filePath);

        std::shared_ptr<Image> get() { return this->texImg; }

        void clearBindlessIndex() { this->delta = index; }
        uint32_t getBindlessIndex() const { return (index - delta); }
        uint32_t getIndex() const { return this->index; }
        static void ResetIndex() { nextTextureIndex = 0; }

      private:
        std::shared_ptr<Image> texImg;
        uint32_t index{0};
        uint32_t delta{0};

        inline static uint32_t nextTextureIndex = 0;
    };

    struct TextureLoader {
        using result_type = std::shared_ptr<VulkanTexture>;
        std::shared_ptr<VulkanTexture> operator()(std::shared_ptr<VulkanContext> ctx,
                                                  const std::string& filePath) const {
            return VulkanTexture::create(ctx, filePath);
        }
    };

} // namespace ce
