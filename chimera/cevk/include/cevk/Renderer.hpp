#pragma once
#include "subsystem.hpp"
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
