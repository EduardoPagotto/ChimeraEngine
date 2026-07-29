#pragma once

#include "CmdBuffer.hpp"
#include "MeshModel.hpp"
#include "Pipeline.hpp"
#include "SwapChain.hpp"
#include "Sync.hpp"
#include "Textures.hpp"
#include "UBO.hpp"
#include "cevk.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class VulkanRenderer {
  public:
    explicit VulkanRenderer(ce::VulkanContext& context);
    virtual ~VulkanRenderer();

    void updateModel(int modelId, glm::mat4 newModel);
    int createMeshModel(const std::string& modelFile);
    void draw();

  private:
    int currentFrame = 0;

    VkPushConstantRange pushConstantRange;

    // Scene Settings
    struct UboViewProjection {
        glm::mat4 projection;
        glm::mat4 view;
    } uboViewProjection;

    ce::VulkanContext& context;

    std::shared_ptr<ce::SwapChain> swapchain;

    std::vector<ce::CmdBuffer> cmdBuffers;
    ce::DescriptorPool descriptorPool;
    ce::UniformBuffer uniformBufferVP;

    std::shared_ptr<ce::Textures> textureMng;
    std::shared_ptr<ce::PipelineLayout> pipelineLayout;
    std::shared_ptr<ce::Pipeline> graphicPipeline;
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
};
