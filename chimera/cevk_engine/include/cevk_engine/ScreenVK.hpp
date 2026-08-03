#pragma once
#include "cevk/CmdBuffer.hpp"
#include "cevk/SwapChain.hpp"
#include "cevk/Sync.hpp"
#include "cevk/VulkanContext.hpp"
#include "cevk_infra/IScr.hpp"
#include "cevk_infra/ServiceLocator.hpp"
#include <SDL3/SDL.h>
#include <memory>

namespace ce {

    /// @brief Canva Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20270801
    class ScreenVK : public ServiceBase<IScr> {

      public:
        explicit ScreenVK(std::shared_ptr<VulkanContext> ctx);
        virtual ~ScreenVK();

        virtual void before() override;
        virtual void after() override;
        virtual void toggleFullScreen() override;
        virtual void reshape(int _width, int _height) override;
        virtual const int getWidth() const override;
        virtual const int getHeight() const override;

        ce::SwapChain& getSwapchain() { return this->swapchain; }
        std::vector<ce::CmdBuffer>& getCmdBuffers() { return this->cmdBuffers; }
        std::vector<ce::Sync>& getSyncs() { return this->syncs; }

      private:
        std::shared_ptr<VulkanContext> ctx;

        ce::SwapChain swapchain;
        std::vector<ce::CmdBuffer> cmdBuffers;
        std::vector<ce::Sync> syncs;

        // VkPushConstantRange pushConstantRange;

        // // Scene Settings
        // struct UboViewProjection {
        //     glm::mat4 projection;
        //     glm::mat4 view;
        // } uboViewProjection;

        // ce::DescriptorPool descriptorPool;
        // ce::UniformBuffer uniformBufferVP;

        // std::shared_ptr<ce::Textures> textureMng;

        // std::shared_ptr<ce::PipelineLayout> pipelineLayout;
        // std::shared_ptr<ce::Pipeline> graphicPipeline;

        // // Scene Objects
        // std::vector<ce::MeshModel> modelList;
    };
} // namespace ce
