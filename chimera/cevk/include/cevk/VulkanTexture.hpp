#pragma once

#include "Image.hpp"
#include "VulkanContext.hpp"
#include <SDL3_image/SDL_image.h>
#include <memory>

namespace ce {

    class VulkanTexture {

      public:
        VulkanTexture(std::shared_ptr<Image> tex_img) : tex_img_(tex_img), index_(next_texture_index++) {}
        ~VulkanTexture() { tex_img_.reset(); }

        // Factory pattern exigido pelo AssetManager
        static std::shared_ptr<VulkanTexture> create(std::shared_ptr<VulkanContext> ctx, const std::string& file_path);

        std::shared_ptr<Image> get() { return this->tex_img_; }

        void clear_bindless_index() { this->delta_ = index_; }
        uint32_t get_bindless_index() const { return (index_ - delta_); }
        uint32_t get_index() const { return this->index_; }
        static void reset_index() { next_texture_index = 0; }

      private:
        std::shared_ptr<Image> tex_img_;
        uint32_t index_{0};
        uint32_t delta_{0};

        inline static uint32_t next_texture_index = 0;
    };

    struct TextureLoader {
        using result_type = std::shared_ptr<VulkanTexture>;
        std::shared_ptr<VulkanTexture> operator()(std::shared_ptr<VulkanContext> ctx,
                                                  const std::string& file_path) const {
            return VulkanTexture::create(ctx, file_path);
        }
    };

} // namespace ce
