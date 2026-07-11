#ifndef __RENDERER_CLASS_HPP
#define __RENDERER_CLASS_HPP

#include "cevk.hpp"

namespace ce {

    class Renderer {
      public:
        explicit Renderer(BaseVK* pBVK, const VkFormat& format);
        virtual ~Renderer();

        VkRenderPass& getRenderPass() { return renderPass; }

      private:
        VkPhysicalDevice physical{VK_NULL_HANDLE};
        VkDevice logical{VK_NULL_HANDLE};

        VkRenderPass renderPass;

        void createRenderPass(const VkFormat& format);
    };
} // namespace ce
#endif
