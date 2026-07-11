#ifndef __RENDERER_CLASS_HPP
#define __RENDERER_CLASS_HPP

#include "cevk.hpp"
#include <memory>

namespace ce {

    class Renderer {
      public:
        explicit Renderer(std::shared_ptr<BaseVK> bvk, const VkFormat& format);
        virtual ~Renderer();

        VkRenderPass& getRenderPass() { return renderPass; }

      private:
        std::shared_ptr<BaseVK> bvk;

        VkRenderPass renderPass;

        void createRenderPass(const VkFormat& format);
    };
} // namespace ce
#endif
