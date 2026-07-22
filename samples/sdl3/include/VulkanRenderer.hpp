#pragma once

#include "CmdBuffer.hpp"
#include "CommandPool.hpp"
#include "DevVK.hpp"
#include "MeshModel.hpp"
#include "Pipeline.hpp"
#include "SwapChain.hpp"
#include "Sync.hpp"
#include "Textures.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class VulkanRenderer {
  public:
    explicit VulkanRenderer(ce::DevVk& devvk);
    virtual ~VulkanRenderer();

    void updateModel(int modelId, glm::mat4 newModel);
    int createMeshModel(const std::string& modelFile);
    void draw();

  private:
    int currentFrame = 0;

    VkQueue gQueue; // graphicsQueue
    VkQueue pQueue; // presentationQueue
    VkPushConstantRange pushConstantRange;

    // Scene Settings
    struct UboViewProjection {
        glm::mat4 projection;
        glm::mat4 view;
    } uboViewProjection;

    std::shared_ptr<ce::BaseVK> bvk;
    std::shared_ptr<ce::SwapChain> swapchain;
    std::vector<ce::CmdBuffer> cmdBuffers;
    std::shared_ptr<ce::DescriptorPool> descriptorPool;
    std::shared_ptr<ce::UBO<ce::Buffer>> uboVP;
    std::shared_ptr<ce::Textures> textureMng;
    std::shared_ptr<ce::PipelineLayout> pipelineLayout;
    std::shared_ptr<ce::Pipeline> graphicPipeline;
    std::shared_ptr<ce::CommandPool> graphicsCommandPool;
    std::vector<ce::Sync> syncs;
    // Scene Objects
    std::vector<ce::MeshModel> modelList;

    // - Vulkan create functions
    void createDescriptorSetLayout();
    void createPushConstantRange();
    void createGraphicsPipeline();
    void createCommandPool();
    void createDescriptorPool();
    void createDescriptorSets();

    // - Record Functions
    void recordCommands(uint32_t currentImage);
};
